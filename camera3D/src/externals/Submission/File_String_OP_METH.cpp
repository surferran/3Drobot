#include "stdafx.h"

//1 FILE_OP METHODEN###########################################################################

void File_OP::OpenWriter(string filename,bool append)
{
  my_filename = filename;
  this->OpenWriter(append);
}
void File_OP::OpenWriter(bool append)
{
	if(file_out.is_open())
	{
	 file_out.close();
	}
	if(append == true)
		mode = ios_base::out|ios_base::app;
	else
		mode = ios_base::out;
	file_out.open(my_filename.c_str(),mode);

}

void File_OP::WriteLine(string line)
{
	file_out << line << endl;
}
void File_OP::Write(string strX)
{
   file_out << strX;
}
void File_OP::CloseStream()
{
   if (file_out.is_open())
   {
       file_out.clear();
	   file_out.close();
   }
   else if ( file_in.is_open())
   {   
	   file_in.clear();
	   file_in.close();
   }
   my_filename.clear();
 
}
void File_OP::OpenReader(string filename)
{  if(file_in.is_open())
	{
	 file_in.close();
	}
  my_filename = filename;
  file_in.open(my_filename.c_str(),ios_base::in);
}
string File_OP::ReadLine()
{
	 {  string helpstr;
     char zeichen = NULL;
	 while( zeichen != '\n' && !file_in.eof())
	 {
       file_in.get(zeichen);
      helpstr += zeichen;
	  }

	 if (helpstr.size() == 0 || helpstr == "\n")
	    return "LEER";
    if (file_in.eof())
	    return "EOF";
    return helpstr;
  }
 
 
}
string File_OP::Read()
{
	 {  string helpstr;
     char zeichen = NULL;
	 while( zeichen != '\n' && !file_in.eof())
	 {
      file_in.get(zeichen);
	   if(zeichen != '\n')
	     helpstr += zeichen;
	   else
		   break;
	  }

	 if (helpstr.size() == 0 || helpstr == "\n")
	    return "LEER";
    if (file_in.eof())
	    return "EOF";
    return helpstr;
  }
 
 
}
void File_OP::ReadFileIntoVector()
{
  line_of_file.clear();
  char zeichen ;
  string helpstr;
  while(!file_in.eof())
  {
     file_in.get(zeichen);
	 if(zeichen != '\n')
     helpstr += zeichen;
	 else if(zeichen == '\n')
	 {
	   line_of_file.push_back(helpstr);
	   helpstr.clear();
	 }
  // file_in >> helpstr;

  // line_of_file.push_back(helpstr);
  // helpstr.clear();
  }
  it_line = line_of_file.begin();
  
}

string File_OP::ReadFromVector()
{
   if(it_line == line_of_file.end())
   { 
     it_line = line_of_file.begin();
     return "EOF";
   }
   else
   { 
    return *it_line++;
   }
   
}

int File_OP::GetVectorSize()
{
  return line_of_file.size();
}
string File_OP::ReadFromVector(int index)
{
  return line_of_file[index];
}


//STRING_OP Methoden######################################################################################

//STRING_OP Methoden

vector<string> String_OP::Get_Tokens(string & str, string & delim)
{
  Tokenize(str, delim);
  return str_vector;
}
vector<string> String_OP::Get_Tokens()
{
 return str_vector;
}

void String_OP::Tokenize(string & str, string & delim)
{
	
	str_vector.clear();
	string::size_type lastPos = str.find_first_not_of(delim,0);
	string::size_type pos = str.find_first_of(delim,lastPos);

    while(string::npos != pos || string::npos != lastPos)
	{
	  str_vector.push_back(str.substr(lastPos,pos-lastPos));
    	lastPos = str.find_first_not_of(delim,pos);
    	pos = str.find_first_of(delim, lastPos);
	}
}

void String_OP::PrintStrings()
{
   	for(it = str_vector.begin();it!= str_vector.end(); it++)
	{	
	cout <<  *it << endl;
	}

}
string  String_OP::AddInteger(string & str,int cipher)
{
   char buf[9];
   sprintf(buf,"%d",(int) cipher);
  
   return str += buf; 
}
int String_OP::ConvertToInteger(string str)
{
	return atoi(str.c_str());
}
string String_OP::Get_Token(int index)
{
  if(str_vector.size() == 0 || index < 0 || index > str_vector.size())
	  return "ERR";
  else
	  return str_vector[index];
}

string String_OP::Int_to_String(int cipher)
{
	char buf[9];
	sprintf(buf,"%d",(int) cipher);
    string str = buf; 
    return str;
}

string String_OP::IntVector_to_String(vector<int> &vec, string & separator)
{
	string str;
  for(unsigned int x = 0; x < vec.size() ; x++)
  {
     str += this->Int_to_String(vec[x]) + separator;
  }
  return str;
}
string String_OP::IntArr_to_String(int arr[],int arr_lenght,string & separator)
{
   	string str;
  for(int x = 0; x < arr_lenght ; x++)
  {
     str += this->Int_to_String(arr[x]) + separator;
  }
  return str;
}
vector<int> String_OP:: String_to_IntVector(string str, string & separator)
{
	vector<int> vec;
	string::size_type lastPos = str.find_first_not_of(separator,0);
	string::size_type pos = str.find_first_of(separator,lastPos);

    while(string::npos != pos || string::npos != lastPos)
	{
	  vec.push_back(this->ConvertToInteger(str.substr(lastPos,pos-lastPos)));
    	lastPos = str.find_first_not_of(separator,pos);
    	pos = str.find_first_of(separator, lastPos);
	}
	return vec;
}
int String_OP::Get_Size()
{
  return str_vector.size();

}
string String_OP::Internal_Vector_as_String()
{
  	string str;
	for(it = str_vector.begin();it!= str_vector.end(); it++)
	{	
	str += *it;
	}
	return str;
}