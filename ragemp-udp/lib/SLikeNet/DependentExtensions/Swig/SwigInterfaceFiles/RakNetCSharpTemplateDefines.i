//
// This file was taken from RakNet 4.082.
// Please see licenses/RakNet license.txt for the underlying license and related copyright.
//
//
//
// Modified work: Copyright (c) 2018, SLikeSoft UG (haftungsbeschränkt)
//
// This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
// license found in the license.txt file in the root directory of this source tree.

//---------------------------------Template Defines-------------------------
//Swig needs to manually define templates you wish to use, this is done here.

%template(Serialize) SLNet::BitStream::Serialize <bool>;
%template(Serialize) SLNet::BitStream::Serialize <unsigned char>;
%template(Serialize) SLNet::BitStream::Serialize <short>;
%template(Serialize) SLNet::BitStream::Serialize <unsigned short>;
%template(Serialize) SLNet::BitStream::Serialize <long>;
%template(Serialize) SLNet::BitStream::Serialize <long long>;
%template(Serialize) SLNet::BitStream::Serialize <float>;
%template(Serialize) SLNet::BitStream::Serialize <SLNet::RakString>;
%template(Serialize) SLNet::BitStream::Serialize <RakNetGUID>;
%template(Serialize) SLNet::BitStream::Serialize <uint24_t>;

%template(SerializeDelta) SLNet::BitStream::SerializeDelta <bool>;
%template(SerializeDelta) SLNet::BitStream::SerializeDelta <unsigned char>;
%template(SerializeDelta) SLNet::BitStream::SerializeDelta <short>;
%template(SerializeDelta) SLNet::BitStream::SerializeDelta <unsigned short>;
%template(SerializeDelta) SLNet::BitStream::SerializeDelta <long>;
%template(SerializeDelta) SLNet::BitStream::SerializeDelta <long long>;
%template(SerializeDelta) SLNet::BitStream::SerializeDelta <float>;
%template(SerializeDelta) SLNet::BitStream::SerializeDelta <SLNet::RakString>;
%template(SerializeDelta) SLNet::BitStream::SerializeDelta <RakNetGUID>;
%template(SerializeDelta) SLNet::BitStream::SerializeDelta <uint24_t>;

%template(SerializeCompressed) SLNet::BitStream::SerializeCompressed <bool>;
%template(SerializeCompressed) SLNet::BitStream::SerializeCompressed <unsigned char>;
%template(SerializeCompressed) SLNet::BitStream::SerializeCompressed <short>;
%template(SerializeCompressed) SLNet::BitStream::SerializeCompressed <unsigned short>;
%template(SerializeCompressed) SLNet::BitStream::SerializeCompressed <long>;
%template(SerializeCompressed) SLNet::BitStream::SerializeCompressed <long long>;
%template(SerializeCompressed) SLNet::BitStream::SerializeCompressed <float>;
%template(SerializeCompressed) SLNet::BitStream::SerializeCompressed <SLNet::RakString>;
%template(SerializeCompressed) SLNet::BitStream::SerializeCompressed <RakNetGUID>;
%template(SerializeCompressed) SLNet::BitStream::SerializeCompressed <uint24_t>;

%template(SerializeCompressedDelta) SLNet::BitStream::SerializeCompressedDelta <bool>;
%template(SerializeCompressedDelta) SLNet::BitStream::SerializeCompressedDelta <unsigned char>;
%template(SerializeCompressedDelta) SLNet::BitStream::SerializeCompressedDelta <short>;
%template(SerializeCompressedDelta) SLNet::BitStream::SerializeCompressedDelta <unsigned short>;
%template(SerializeCompressedDelta) SLNet::BitStream::SerializeCompressedDelta <long>;
%template(SerializeCompressedDelta) SLNet::BitStream::SerializeCompressedDelta <long long>;
%template(SerializeCompressedDelta) SLNet::BitStream::SerializeCompressedDelta <float>;
%template(SerializeCompressedDelta) SLNet::BitStream::SerializeCompressedDelta <SLNet::RakString>;
%template(SerializeCompressedDelta) SLNet::BitStream::SerializeCompressedDelta <RakNetGUID>;
%template(SerializeCompressedDelta) SLNet::BitStream::SerializeCompressedDelta <uint24_t>;

%template(Write) SLNet::BitStream::Write <const char *>;
%template(Write) SLNet::BitStream::Write <bool>;
%template(Write) SLNet::BitStream::Write <unsigned char>;
%template(Write) SLNet::BitStream::Write <char>;
%template(Write) SLNet::BitStream::Write <short>;
%template(Write) SLNet::BitStream::Write <unsigned short>;
%template(Write) SLNet::BitStream::Write <long>;
%template(Write) SLNet::BitStream::Write <long long>;
%template(Write) SLNet::BitStream::Write <float>;
%template(Write) SLNet::BitStream::Write <SLNet::RakString>;
%template(Write) SLNet::BitStream::Write <RakNetGUID>;
%template(Write) SLNet::BitStream::Write <uint24_t>;

%template(WriteDelta) SLNet::BitStream::WriteDelta <const char *>;
%template(WriteDelta) SLNet::BitStream::WriteDelta <bool>;
%template(WriteDelta) SLNet::BitStream::WriteDelta <unsigned char>;
%template(WriteDelta) SLNet::BitStream::WriteDelta <char>;
%template(WriteDelta) SLNet::BitStream::WriteDelta <short>;
%template(WriteDelta) SLNet::BitStream::WriteDelta <unsigned short>;
%template(WriteDelta) SLNet::BitStream::WriteDelta <long>;
%template(WriteDelta) SLNet::BitStream::WriteDelta <long long>;
%template(WriteDelta) SLNet::BitStream::WriteDelta <float>;
%template(WriteDelta) SLNet::BitStream::WriteDelta <SLNet::RakString>;
%template(WriteDelta) SLNet::BitStream::WriteDelta <RakNetGUID>;
%template(WriteDelta) SLNet::BitStream::WriteDelta <uint24_t>;

%template(WriteCompressed) SLNet::BitStream::WriteCompressed <const char*>;
%template(WriteCompressed) SLNet::BitStream::WriteCompressed <bool>;
%template(WriteCompressed) SLNet::BitStream::WriteCompressed <unsigned char>;
%template(WriteCompressed) SLNet::BitStream::WriteCompressed <char>;
%template(WriteCompressed) SLNet::BitStream::WriteCompressed <short>;
%template(WriteCompressed) SLNet::BitStream::WriteCompressed <unsigned short>;
%template(WriteCompressed) SLNet::BitStream::WriteCompressed <long>;
%template(WriteCompressed) SLNet::BitStream::WriteCompressed <long long>;
%template(WriteCompressed) SLNet::BitStream::WriteCompressed <float>;
%template(WriteCompressed) SLNet::BitStream::WriteCompressed <SLNet::RakString>;
%template(WriteCompressed) SLNet::BitStream::WriteCompressed <RakNetGUID>;
%template(WriteCompressed) SLNet::BitStream::WriteCompressed <uint24_t>;

%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <const char *>;
%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <bool>;
%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <unsigned char>;
%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <char>;
%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <short>;
%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <unsigned short>;
%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <long>;
%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <long long>;
%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <float>;
%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <SLNet::RakString>;
%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <RakNetGUID>;
%template(WriteCompressedDelta) SLNet::BitStream::WriteCompressedDelta <uint24_t>;

%template(Read) SLNet::BitStream::Read <bool>;
%template(Read) SLNet::BitStream::Read <unsigned char>;
%template(Read) SLNet::BitStream::Read <short>;
%template(Read) SLNet::BitStream::Read <unsigned short>;
%template(Read) SLNet::BitStream::Read <long>;
%template(Read) SLNet::BitStream::Read <long long>;
%template(Read) SLNet::BitStream::Read <float>;
%template(Read) SLNet::BitStream::Read <SLNet::RakString>;
%template(Read) SLNet::BitStream::Read <RakNetGUID>;
%template(Read) SLNet::BitStream::Read <uint24_t>;

%template(ReadDelta) SLNet::BitStream::ReadDelta <bool>;
%template(ReadDelta) SLNet::BitStream::ReadDelta <unsigned char>;
%template(ReadDelta) SLNet::BitStream::ReadDelta <short>;
%template(ReadDelta) SLNet::BitStream::ReadDelta <unsigned short>;
%template(ReadDelta) SLNet::BitStream::ReadDelta <long>;
%template(ReadDelta) SLNet::BitStream::ReadDelta <long long>;
%template(ReadDelta) SLNet::BitStream::ReadDelta <float>;
%template(ReadDelta) SLNet::BitStream::ReadDelta <SLNet::RakString>;
%template(ReadDelta) SLNet::BitStream::ReadDelta <RakNetGUID>;
%template(ReadDelta) SLNet::BitStream::ReadDelta <uint24_t>;

%template(ReadCompressed) SLNet::BitStream::ReadCompressed <bool>;
%template(ReadCompressed) SLNet::BitStream::ReadCompressed <unsigned char>;
%template(ReadCompressed) SLNet::BitStream::ReadCompressed <short>;
%template(ReadCompressed) SLNet::BitStream::ReadCompressed <unsigned short>;
%template(ReadCompressed) SLNet::BitStream::ReadCompressed <long>;
%template(ReadCompressed) SLNet::BitStream::ReadCompressed <long long>;
%template(ReadCompressed) SLNet::BitStream::ReadCompressed <float>;
%template(ReadCompressed) SLNet::BitStream::ReadCompressed <SLNet::RakString>;
%template(ReadCompressed) SLNet::BitStream::ReadCompressed <RakNetGUID>;
%template(ReadCompressed) SLNet::BitStream::ReadCompressed <uint24_t>;

%template(ReadCompressedDelta) SLNet::BitStream::ReadCompressedDelta <bool>;
%template(ReadCompressedDelta) SLNet::BitStream::ReadCompressedDelta <unsigned char>;
%template(ReadCompressedDelta) SLNet::BitStream::ReadCompressedDelta <short>;
%template(ReadCompressedDelta) SLNet::BitStream::ReadCompressedDelta <unsigned short>;
%template(ReadCompressedDelta) SLNet::BitStream::ReadCompressedDelta <long>;
%template(ReadCompressedDelta) SLNet::BitStream::ReadCompressedDelta <long long>;
%template(ReadCompressedDelta) SLNet::BitStream::ReadCompressedDelta <float>;
%template(ReadCompressedDelta) SLNet::BitStream::ReadCompressedDelta <SLNet::RakString>;
%template(ReadCompressedDelta) SLNet::BitStream::ReadCompressedDelta <RakNetGUID>;
%template(ReadCompressedDelta) SLNet::BitStream::ReadCompressedDelta <uint24_t>;

%define ADD_LIST_TYPE(CTYPE,CSTYPE,RENAME_TYPE)
%typemap(cscode) DataStructures::List<CTYPE>
%{
    public CSTYPE this[int index]  
    {  
        get   
        {
            return Get((uint)index); // use indexto retrieve and return another value.    
        }  
        set   
        {
            Replace(value, value, (uint)index, "Not used", 0);// use index and value to set the value somewhere.   
        }  
    } 
%}

%template(RENAME_TYPE) DataStructures::List <CTYPE>;
%enddef

ADD_LIST_TYPE(SLNet::RakNetGUID,RakNetGUID,RakNetListRakNetGUID)
ADD_LIST_TYPE(SLNet::SystemAddress,SystemAddress,RakNetListSystemAddress)
ADD_LIST_TYPE(SLNet::RakString,RakString,RakNetListRakString)
ADD_LIST_TYPE(Cell,Cell,RakNetListCell)
ADD_LIST_TYPE(ColumnDescriptor,ColumnDescriptor,RakNetListColumnDescriptor)
ADD_LIST_TYPE(Row,Row,RakNetListTableRow);
ADD_LIST_TYPE(SLNet::FileListNode,FileListNode,RakNetListFileListNode);
ADD_LIST_TYPE(FilterQuery,FilterQuery,RakNetListFilterQuery);
ADD_LIST_TYPE(SortQuery,SortQuery,RakNetListSortQuery);

// 1/1/2011 Commented out below line: Doesn't build into RakNet_wrap.cxx properly
// %template(RakNetSmartPtrRakNetSocket) RakNetSmartPtr<RakNetSocket>;

//Can't use the macro because it won't include the space then nested templates won't work
/*
%typemap(cscode) DataStructures::List<RakNetSmartPtr<RakNetSocket> >
%{
    public RakNetSmartPtrRakNetSocket this[int index]  
    {  
        get   
        {
            return Get((uint)index); // use indexto retrieve and return another value.    
        }  
        set   
        {
            Replace(value, value, (uint)index, "Not used", 0);// use index and value to set the value somewhere.   
        }  
    } 
%}

%template(RakNetListRakNetSmartPtrRakNetSocket) DataStructures::List <RakNetSmartPtr<RakNetSocket> >;
*/

%define ADD_POINTER_LIST_TYPE(CTYPE,CSTYPE,RENAME_TYPE)
%ignore DataStructures::List<CTYPE>::Get;
%ignore DataStructures::List<CTYPE>::Pop;

%typemap(cscode) DataStructures::List<CTYPE>
%{
    public CSTYPE this[int index]  
    {  
        get   
        {
            return Get((uint)index); // use indexto retrieve and return another value.    
        }  
        set   
        {
            Replace(value, value, (uint)index, "Not used", 0);// use index and value to set the value somewhere.   
        }  
    }


    public CSTYPE Get(uint position) 
    {   
	return GetHelper(position);
    }

    public CSTYPE Pop()
    {
	return PopHelper();
    }

%}

%extend DataStructures::List<CTYPE>
{
	CTYPE GetHelper ( const unsigned int position ) const
	{
		return self->Get(position);
        }


	CTYPE PopHelper () 
	{
		return self->Pop();
        }
}

%template(RENAME_TYPE) DataStructures::List <CTYPE>;
%enddef

ADD_POINTER_LIST_TYPE(Cell *,Cell,RakNetListCellPointer)
#ifdef SWIG_ADDITIONAL_AUTOPATCHER
	ADD_POINTER_LIST_TYPE(AutopatcherRepositoryInterface *,AutopatcherRepositoryInterface,RakNetListAutopatcherRepositoryInterfacePointer)
#endif

%define ADD_PRIMITIVE_LIST_TYPE(CTYPE,CSTYPE,RENAME_TYPE,SWIG_TYPE,POINTER_NAME)
%pointer_class(CTYPE, POINTER_NAME)

%csmethodmodifiers DataStructures::List <CTYPE>::Get "private"
%csmethodmodifiers DataStructures::List <CTYPE>::Pop "private"

%rename(GetHelper) DataStructures::List <CTYPE>::Get;
%rename(PopHelper) DataStructures::List <CTYPE>::Pop;

%typemap(cscode) DataStructures::List <CTYPE>
%{
  public CSTYPE Get(uint position) {
    SWIG_TYPE ret = GetHelper(position);
    return POINTER_NAME.frompointer(ret).value();
  }

  public CSTYPE Pop() {
    SWIG_TYPE ret = PopHelper();
    return POINTER_NAME.frompointer(ret).value();
  }
    public CSTYPE this[int index]  
    {  
        get   
        {
            return Get((uint)index); // use indexto retrieve and return another value.    
        }  
        set   
        {
            Replace(value, value, (uint)index, "Not used", 0);// use index and value to set the value somewhere.   
        }  
    } 
%}

%template(RENAME_TYPE) DataStructures::List <CTYPE>;
%enddef

ADD_PRIMITIVE_LIST_TYPE(unsigned short,ushort,RakNetListUnsignedShort,SWIGTYPE_p_unsigned_short,UnsignedShortPointer)
ADD_PRIMITIVE_LIST_TYPE(unsigned,uint,RakNetListUnsignedInt,SWIGTYPE_p_unsigned_int,UnsignedIntPointer)

%template(RakNetPageRow) DataStructures::Page<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER>;

//needed here for scoping issues
%ignore DataStructures::BPlusTree<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER>::Delete;
%ignore DataStructures::BPlusTree<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER>::Get;

%csmethodmodifiers DataStructures::BPlusTree<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER>::DeleteHelper "private";
%csmethodmodifiers DataStructures::BPlusTree<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER>::GetHelper "private";

%template(RakNetBPlusTreeRow) DataStructures::BPlusTree<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER>;

%typemap(cscode) DataStructures::BPlusTree<unsigned, DataStructures::Table::Row*, _TABLE_BPLUS_TREE_ORDER>
%{
	public bool Get(uint key, ref Row arg1) 
	{
		bool outBool;

		arg1=GetHelper(key,arg1,out outBool);

  		return outBool;
  	}

  	public bool Delete(uint key) 
	{
    		return DeleteHelper(key);
  	}

	public bool Delete(uint key, ref Row arg1) 
	{
		bool outBool;
		
		arg1=DeleteHelper(key,arg1,out outBool);

  		return outBool;
 	}
%}

%define ADD_STANDARD_MULTILIST_TYPE(CTYPE,CSTYPE,RENAME_TYPE)
%typemap(cscode) DataStructures::Multilist <ML_STACK,CTYPE,CTYPE,DefaultIndexType>
%{
    public CSTYPE this[int index]  
    {  
        get   
        {
            return OpArray((uint)index); // use indexto retrieve and return another value.    
        }  
        set   
        {
            RemoveAtIndex((uint)index);
            InsertAtIndex(value,(uint)index);
	}  
    } 
%}

%template(RENAME_TYPE) DataStructures::Multilist <ML_STACK,CTYPE,CTYPE,DefaultIndexType>;
%enddef

#ifdef SWIG_ADDITIONAL_SQL_LITE
ADD_STANDARD_MULTILIST_TYPE(SQLite3Row*,SQLite3Row,RakNetMultiListML_StackSQLite3RowP)
ADD_STANDARD_MULTILIST_TYPE(SLNet::RakString,RakString,RakNetMultiListML_StackRakString)
ADD_STANDARD_MULTILIST_TYPE(SLNet::SQLite3PluginResultInterface *,SQLite3PluginResultInterface,RakNetMultiListML_StackSQLite3PluginResultInterfaceP)
#endif
