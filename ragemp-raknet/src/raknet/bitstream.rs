use super::bindings as sys;

use std::fmt;
use std::marker::PhantomData;

pub struct BitStream<'a> {
    pub bs: Box<sys::BitStream>,
    _marker: PhantomData<&'a ()>,
}

impl<'a> BitStream<'a> {
    pub fn new() -> BitStream<'a> {
        let bs = unsafe {
            let bs = Box::new(sys::BitStream::new());
            let bs = Box::into_raw(bs);
            (*bs).data = (*bs).stackData.as_mut_ptr();

            Box::from_raw(bs)
        };

        BitStream {
            bs,
            _marker: PhantomData,
        }
    }

    pub fn new_with_capacity(capacity: usize) -> BitStream<'a> {
        let bs = unsafe {
            let bs = Box::new(sys::BitStream::new1(capacity as u32));

            if capacity <= bs.stackData.len() {
                let bs = Box::into_raw(bs);
                (*bs).data = (*bs).stackData.as_mut_ptr();

                Box::from_raw(bs)
            } else {
                bs
            }
        };

        BitStream {
            bs,
            _marker: PhantomData,
        }
    }

    pub fn write_bit(&mut self, enabled: bool) {
        unsafe {
            if enabled {
                self.bs.WriteBit1();
            } else {
                self.bs.WriteBit0();
            }
        }
    }

    pub fn write<T: Sized>(&mut self, data: &T) {
        let size = std::mem::size_of::<T>();
        let data_ptr = data as *const T as *const i8;

        unsafe {
            self.bs.Write(data_ptr, size as u32);
        }
    }

    pub fn write_bits<T: Sized>(&mut self, data: &T, bits: usize, right_aligned: bool) {
        let size = std::mem::size_of::<T>();
        let bits = std::cmp::min(bits, size << 3) as u32;
        let data_ptr = data as *const T as *const u8;

        unsafe {
            self.bs.WriteBits(data_ptr, bits, right_aligned);
        }
    }

    pub fn write_bytes(&mut self, slice: &[u8]) {
        let data_ptr = slice.as_ptr() as *const i8;
        unsafe {
            self.bs.Write(data_ptr, slice.len() as u32);
        }
    }

    pub fn write_aligned_bytes(&mut self, slice: &[u8]) {
        let data_ptr = slice.as_ptr();
        unsafe {
            self.bs.WriteAlignedBytes(data_ptr, slice.len() as u32);
        }
    }

    pub fn write_string(&mut self, string: &str) {
        let bytes = string.as_bytes();
        let length = bytes.len() as u16;
        self.write(&length);
        self.write_aligned_bytes(bytes);
    }

    pub fn read_bit(&mut self) -> bool {
        unsafe { self.bs.ReadBit() }
    }

    pub fn read<T: Sized + Default>(&mut self) -> Option<T> {
        let size = std::mem::size_of::<T>();
        let mut default = T::default();
        let result = unsafe {
            self.bs
                .Read4(&mut default as *mut _ as *mut i8, size as u32)
        };

        if result {
            Some(default)
        } else {
            None
        }
    }

    pub fn read_bits<T: Sized + Default>(&mut self, bits: usize, right_aligned: bool) -> Option<T> {
        let size = std::mem::size_of::<T>();
        let bits = std::cmp::min(bits, size << 3) as u32;
        let mut default = T::default();
        let result = unsafe {
            self.bs
                .ReadBits(&mut default as *mut _ as *mut u8, bits, right_aligned)
        };

        if result {
            Some(default)
        } else {
            None
        }
    }

    pub fn read_bytes(&mut self, length: usize) -> Option<Vec<u8>> {
        let mut vec: Vec<u8> = vec![0; length];
        let result = unsafe { self.bs.Read4(vec.as_mut_ptr() as *mut i8, length as u32) };

        if result {
            Some(vec)
        } else {
            None
        }
    }

    pub fn read_aligned_bytes(&mut self, length: usize) -> Option<Vec<u8>> {
        let mut vec: Vec<u8> = vec![0; length];
        let result = unsafe { self.bs.ReadAlignedBytes(vec.as_mut_ptr(), length as u32) };

        if result {
            Some(vec)
        } else {
            None
        }
    }

    pub fn read_string(&mut self) -> Option<String> {
        let length = self.read::<u16>()?;
        let bytes = self.read_aligned_bytes(length as usize)?;

        String::from_utf8(bytes).ok()
    }

    pub fn ignore_bits(&mut self, bits: usize) {
        unsafe {
            self.bs.IgnoreBits(bits as u32);
        }
    }

    pub fn ignore_bytes(&mut self, bytes: usize) {
        unsafe {
            self.bs.IgnoreBytes(bytes as u32);
        }
    }

    pub fn reset(&mut self) {
        unsafe {
            self.bs.Reset();
        }
    }

    pub fn reset_write_pointer(&mut self) {
        unsafe {
            self.bs.ResetWritePointer();
        }
    }

    pub fn reset_read_pointer(&mut self) {
        unsafe {
            self.bs.ResetReadPointer();
        }
    }

    pub fn print_hex(&self) {
        unsafe {
            self.bs.PrintHex1();
            println!("");
        }
    }

    pub fn inner(&self) -> &sys::BitStream {
        self.bs.as_ref()
    }

    pub fn inner_mut(&mut self) -> &mut sys::BitStream {
        self.bs.as_mut()
    }

    pub fn clone_static(&'a self) -> BitStream<'static> {
        let slice = unsafe { std::slice::from_raw_parts(self.bs.data, (self.bs.numberOfBitsUsed >> 3) as usize) };
        BitStream::from(slice)
    }
}

impl Drop for BitStream<'_> {
    fn drop(&mut self) {
        unsafe {
            self.bs.destruct();
        }
    }
}

impl fmt::Debug for BitStream<'_> {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        f.debug_struct("BitStream")
            .field("bits_used", &self.bs.numberOfBitsUsed)
            .field("bits_allocated", &self.bs.numberOfBitsAllocated)
            .field("read_offset", &self.bs.readOffset)
            .finish()
    }
}

impl From<&[u8]> for BitStream<'static> {
    /// DOES **COPY** CONTENT FROM A SLICE
    fn from(data: &[u8]) -> BitStream<'static> {
        let mut bs = BitStream::new();
        bs.write_bytes(data);
        bs
    }
}

impl<'a> From<&'a mut [u8]> for BitStream<'a> {
    fn from(data: &'a mut [u8]) -> BitStream<'a> {
        unsafe {
            BitStream {
                bs: Box::new(sys::BitStream::new2(
                    data.as_mut_ptr(),
                    data.len() as u32,
                    false,
                )),
                _marker: PhantomData,
            }
        }
    }
}
