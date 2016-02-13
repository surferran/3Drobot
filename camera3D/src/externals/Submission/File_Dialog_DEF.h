//###### FILE DIALOG CLASS DEFINITIONS (by MKopp) ##########
//Common File Dialog //requires Multibyte string literals (see Project Properties)
//uses std::<map> and std::<string> 
class FileDialog
{
public: 
//Konstruktor //with Constructur list
	FileDialog(HWND parent_handle);
	~FileDialog(){}
//Methods
   //Shows Open File Dialog
	bool OpenFile();
   //Shows Save File Dialog
   bool SaveFile();
   //Opens Folder Only
   bool OpenFileEx();
   //returns Char Array of selected path
   char* Get_Name();

   //EXTRA: methods for loading and accessing files in a folder

   //fills all file names (strings the form -> string_number e.g. Benedict_16) of an folder into map
   void Store_Folders_FilesEx();
   //returns file name without path
   string Get_FilenameEx();
   //returns filenames of list
   string Get_Folder_FilenameEx();
   //returns File path  string
   string Get_PathNameEx();
   //returns size of list containing filenames
   int Get_Map_Size();

   //for testing purposes
   void PrintPathName(int posX, int posY);
   
private:
	OPENFILENAME my_ofn;
	//Struct for filename properties
	WIN32_FIND_DATA ffd;
	//Handle for found file
	HANDLE h_Find ;
	HWND my_hwnd;
	

	char szFileName[260];
	char Path_only[260];
	char *p_szFileName;

	//EXTRA: variables for laoding folder methods
	map<int,string> file_map;
	map<int,string>::iterator it;
	string path_name;
	string dummy_str;
	string file_name_only;
	
};