use std::fs::File;
use std::hash::Hasher;
use std::io::{BufRead, BufReader};
use std::io::{Cursor, Read, Seek, SeekFrom, Write};
use std::path::{Path, PathBuf};

use bytes::{Buf, BufMut};
use twox_hash::XxHash;

const FILENAME: &str = "origin_.js";

pub struct Manipulator {
    origin_host: String,
    origin_files_len: usize,
    main_file_idx: usize,
    script_path: PathBuf,
    script_hash: u64,
}

impl Manipulator {
    pub fn new<P: AsRef<Path>>(path: P, origin_host: String) -> Manipulator {
        let path = path.as_ref();
        let f = File::open(path).unwrap();
        let mut f = BufReader::new(f);
        let mut hasher = XxHash::with_seed(0);

        loop {
            let consumed = {
                let bytes = f.fill_buf().unwrap();

                if bytes.len() == 0 {
                    break;
                }

                hasher.write(bytes);
                bytes.len()
            };

            f.consume(consumed);
        }

        let script_hash = hasher.finish();

        println!("xxxxx: {:16x}", script_hash);

        Manipulator {
            origin_host,
            origin_files_len: 0,
            main_file_idx: 0,
            script_path: path.into(),
            script_hash,
        }
    }

    pub fn generate(&mut self, buf: &mut Cursor<Vec<u8>>) {
        let mut origin_hash = 0u64;
        let mut count = 0;

        buf.set_position(0);
        buf.seek(SeekFrom::Start(0));

        while buf.remaining() > 0 {
            let length = buf.get_u32_le() as usize;
            let mut name = vec![0; length];
            let _ = buf.read_exact(&mut name);
            let hash = buf.get_u64_le() as usize;
            let name = String::from_utf8_lossy(&name);

            if name == "index.js" {
                origin_hash = hash as u64;
                self.main_file_idx = count;
                buf.seek(SeekFrom::Current(-8)); // замена хеша
                println!("origin: {:16x}", origin_hash);
                // buf.get_mut().put_u64_le(self.script_hash);
                buf.write_all(&self.script_hash.to_le_bytes());
            }

            count += 1;
        }

        //добавляем в конец оригинальный index.js с сервера, но уже под другим именем
        let additional_len =
            FILENAME.len() + std::mem::size_of::<u32>() + std::mem::size_of::<u64>();

        let mut append = Cursor::new(vec![]);

        append.set_position(0);
        append.seek(SeekFrom::Start(0));

        append.get_mut().put_u32_le(FILENAME.len() as u32);
        // println!("{}", append.position());
        append.set_position(4);
        let _ = append.write_all(FILENAME.as_bytes());
        append.get_mut().put_u64_le(origin_hash);

        println!("{:?}", append.get_mut());

        println!("{:?}", buf.write_all(&append.into_inner()));

        self.origin_files_len = count;
    }

    pub fn is_origin_script_index(&self, index: usize) -> bool {
        index >= self.origin_files_len
    }

    pub fn is_custom_script_index(&self, index: usize) -> bool {
        index == self.main_file_idx
    }

    pub fn origin_script_index(&self) -> usize {
        self.main_file_idx
    }

    pub fn script_path(&self) -> &Path {
        self.script_path.as_path()
    }

    pub fn origin_host(&self) -> &str {
        &self.origin_host
    }
}
