
// IMPORTANT: HOW TO IMPLEMENT AND MAKE USE OF OPENCV CODE

// ATTENTION: despite of being oldfashioned PROGRAM USES MULTIBYTE INSTEAD OF UNICODE
// find a illustrative tutorial about UNICODE and Multibyte (ansi) at
// http://www.codeproject.com/Articles/76252/What-are-TCHAR-WCHAR-LPSTR-LPWSTR-LPCTSTR-etc

// ATTENTION: Program requires the right codecs to be installed (download a codec package from the internet)

// ATTENTION:  ffdshow might also be needed to run the program (there are free downloads)

/* I used Visual C++ Express Edition 2010 on a Windows 7 machine (if you want to create a x64 application you need to download SDK 7.1),
   but the following explanations, I think, might also apply to older or other versions of Visual C++.
   I created an x86 application and did not try out the x64 version, therefore I can not guarantee that the instructions for x64 will lead to a useable application  

 1)
 Download and install
 add new environmental variables (Umgebungsvariablen); enter path to access opencv dll files, which are needed to process code
       => Press 'New' in Window 'Environmental Variables' 
	      Enter 'Name' (e.g. OPENCV_PATH)
		  and under 'Value' enter the path to the opencv folder containing the nescessary opencv dll's .
		     I used OpenCV 2.4.3 and the x86 platform 
			 that's why I entered ' C:\opencv\build\x86\vc10\bin ' (depends on the folder you have chosen during installation)
			 If you want to use the x64 version
			 it could be ' C:\opencv\build\x64\vc10\bin ' instead 

       => if you prefer not to add a new path to the environmental folder or if it does not work:
	         Then copy all dll's in the above mentioned folder (depending on what you have chosen: x86 or x64)
			 into  the folder where your .exe file is located.
			 You might also need the 'tbb.dll' files which are located somewhere in the opencv folders (just search for tbb.dll)

3)
Add libraries and header files

       => After you have chosen Win32 (or Win32 console). The opencv libraries and headers have to be included to be available.
	   
	   => Select 'Project/Properties'
	         Open 'C/C++' and click on 'General' (Allgemein) and enter path of opencv include directories into the field 'additional dependencies' (zusätzliche Abhängigkeiten)
			 depending on the installation folder this could be 'C:\opencv\build\include' (for an x86 application like here)
			 
			 For a x64 application first open the 'configuration manager' and choose new platform 'x64' (you need SDK 7.1 for that).
			 Then do the same as described above and add the path to the nescessary opencv header files

       => Select 'Project/Properties'
	          Open 'Linker', select 'General' and enter path into opencv libraries using the field 'additional libraries' ,
			  depending on the installation folder this could be 'C:\opencv\build\x86\vc10\lib' for an x86 application like this one here
			  or 'C:\opencv\build\x64\vc10\lib' for an x64 application 

       => Select 'Project/Properties'
	          Open 'Linker', select 'Input' (Eingabe) and add opencv lib. files to the field 'additional dependencies' .
			  e.g. (files with a d at the end of the file's names are the debug versions)
			  This could be 'opencv_core243.lib;opencv_highgui243.lib;opencv_imgproc243.lib;opencv_features2d243.lib;opencv_flann243.lib;%(AdditionalDependencies) '
			  etc. for an x86 application
			  and for x64 appliaction the correpsonding ones from x64 folder described above in Project/Properties/Linker/General
			  

4)
Include opencv header files (here in stdafx.h file)
 
 */

#include "stdafx.h"
#include "UsingOpenCV.h"
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// titlestring
TCHAR szWindowClass[MAX_LOADSTRING];			//classname of main window

// declaration of Functions of this code module
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// Implemented as modeless dialog; runs parallel to main application // see [REF]
//BOOL CALLBACK   Loaded_Files_Proc(HWND, UINT, WPARAM, LPARAM);


//GLOBAL USERDEFINED VARIABLES

HWND hWnd; //handle of Main Window

 //handles for controls // placed on main window; see WM_CREATE
 HWND path_caption_handle;
 HWND pathinfo_handle; 
 HWND from_caption_handle; 
 HWND to_caption_handle;
 HWND from_handle; 
 HWND to_handle;
 HWND button_handle; 
 HMENU menu_handle;		 
 HWND track_bar_handle;
 HWND trackbar_info_handle;
 HWND dialog_handle;
 HWND check_box_handles[4];

// for File Dialog Operations
FileDialog * fd; //Pointer to FileDialog Class, will be intialized in WM_CREATE
char PathName[MAX_PATH]; //char stores pathname of FileDialog



// Variables for Video operations (see also Image_OP and Video_OP in OpenCv_Video_OP DEF and METH)
Video_OP * vio;
char* movie_full_path;
char* save_file_movie;
string movie_file_name;
//Current folder
char lpBuffer[250];


//GLOBAL USERDEFINED FUNCTIONS

int Get_Number(HWND handle);
void Transfer_Num_as_Text(HWND handle, int number);
void Set_Text(HWND handle, char* str);
void Display_String(HWND handle,int where_x, int where_y, char* str);




int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO //Put in code here
	MSG msg;
	HACCEL hAccelTable;

	//Intialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_USINGOPENCV, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// intialize application
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_USINGOPENCV);

    //main message loop
	while (GetMessage(&msg, NULL, 0, 0)) 
	{

	 // if(!IsDialogMessage(dialog_handle,&msg))//EXTRA LINE OF CODE FOR MODELESS DIALOG MESSAGES // see [REF]
	 //{//IF 1
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	 //}//END IF 1
	}

	return (int) msg.wParam;
}



//
//  FUNKTION: MyRegisterClass()
//
//  Purpose: Registriert die Fensterklasse. 
//

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_USINGOPENCV);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_USINGOPENCV;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   Purpose: saves handle of instance and creates main window
//
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   

   hInst = hInstance; // handle of instance

   //hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    //  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL); //OR
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
     CW_USEDEFAULT, 0, 530, 300, NULL, NULL, hInstance, NULL);
   

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigniert, WORD, LONG)
//
// PUPOSE:  handling of messages from main window  
//
//  WM_COMMAND	- Processes message coming from the menu and the controls (e.g. button)
//  WM_PAINT	- Re/Drawing of main window
//  WM_DESTROY	- Ends application
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// handling commands from menu and controls (e.g. buttons)
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;

			case IDC_BUTTON_DO_IT:
				{
			     // checks if user has chosen "Play Movie" or "Write Movie" in Menu Select Command
                 UINT res = GetMenuState(menu_handle,IDM_PLAY_MOV,MF_BYCOMMAND);
				  char buf[15];

				  // x is not used; fills buffer with text of button ("GO")
				  int x = GetWindowText(button_handle,buf,15);

				  // depending on which submenu is selected => Play_Video or Write_Video
				  if (res == MF_CHECKED) //if(1)
				  {
					
					if( strcmp(buf,"GO")==0)//if (2) checks, whether char strings are identical
					 {
			            vio->Play_Video(Get_Number(from_handle),Get_Number(to_handle), Image_OP::my_ROI);
						Set_Text(button_handle,"FIN");
			         }
					 else
					  {
						vio->Stop_Video();
						
					  }		
				  
				  }//End If(1)
				  else //else belonging to if(1)
				  {
				      if( strcmp(buf,"GO")==0)//if (2) checks, whether char strings are identical
					 {
						Set_Text(button_handle,"WRITING");
						if(fd->SaveFile())
						{
						  save_file_movie = fd->Get_Name();
						}

						Set_Text(pathinfo_handle,save_file_movie);
			            if (vio->Write_Video(Get_Number(from_handle),Get_Number(to_handle), Image_OP::my_ROI, save_file_movie))
						Set_Text(button_handle,"GO");
			         }
				  
				  }//End else (1)
				}
	            break;
			
		case IDM_LOAD_MOV:
			{
			 
			 if(fd->OpenFile())
			 {
			    movie_full_path = fd->Get_Name();
			 }
			
			 // sets pointer to selected video file
			 vio->Get_Video_from_File(movie_full_path);
			 // fills textfield with pathinfo of loaded movie
			 Set_Text(pathinfo_handle,movie_full_path);
			 // fills textfield (labeled 'TO') with a loaded video's number of total frames
			 Transfer_Num_as_Text(to_handle,vio->Get_Total_Frames());
			 Set_Text(button_handle,"GO");
			 
			
			}
			break;
			
		
		case IDM_STOP_MOVIE:
			{
				
				vio->Stop_Video();
				Set_Text(button_handle,"ABORTED");
			}
			break;
		case IDM_PLAY_MOV:
			{
			   // switches between menu checked and menu unchecked state
               CheckMenuItem(menu_handle,IDM_PLAY_MOV,MF_CHECKED);
			   CheckMenuItem(menu_handle,IDM_WRITE_MOV,MF_UNCHECKED); 
			}

			break;   
		case IDM_WRITE_MOV:
			{
			   // switches between menu checked and menu unchecked state
				CheckMenuItem(menu_handle,IDM_WRITE_MOV,MF_CHECKED);
				CheckMenuItem(menu_handle,IDM_PLAY_MOV,MF_UNCHECKED); 
			}
			break;
		case IDM_SHOW_FILES:
			{
			  ShowWindow(dialog_handle,SW_SHOW);
			}
		    break;
		// NOT visible in compiled program
        case IDM_GRAB_PICS:
			{
			
			 if(fd->OpenFileEx())
			 {
			 // gets file_name with path
			  movie_full_path = fd->Get_Name();
			  // gets file_name only
			  movie_file_name = fd->Get_FilenameEx();
			  // stores files of a selected folder in dictionary (Standard template library std::map; see File_Dialog class)
			  // filenames should have the following appearance: Name_## (e.g. Karl_5)
			  fd->Store_Folders_FilesEx();
			 }
			 
			 // resets content of list box in modeless dialog // not visible in compiled program (see [REF])
			 LRESULT lResult = SendDlgItemMessage(dialog_handle,ID_LST_LOADED_FILES,
									 LB_RESETCONTENT, 0,0);

			 // creates array with length of std::map
			 string* file_arr = new string[fd->Get_Map_Size()];
			 int count_it =0;
			 string path_str;
			 string help_str;

			 // transfers file_names of map to array
			 while (true)
			 {
				// concatenates path and filename string till end of std::map is reached (in FileDialog class)
				path_str = fd->Get_PathNameEx();
                help_str= fd->Get_Folder_FilenameEx();
				path_str += help_str;
			   if (help_str == "FIN") break;
			   
			   file_arr[count_it] = path_str;
			   count_it++;
			   // inserts file_names in list box of modeless dialog // see [REF]
			    lResult = SendDlgItemMessage(dialog_handle,ID_LST_LOADED_FILES,
		         LB_ADDSTRING,0,(LPARAM) help_str.c_str());
				
			 }
			 // turns single files (=images) into movie
			 vio->Turn_Pics_into_Video(file_arr,fd->Get_Map_Size(),10);
			 delete []file_arr;
			}
			break;
       // NOT visible in compiled program
		case IDM_MOVIE_TO_PICS:
			{
		     // retrieves file and path of movie
			 if(fd->OpenFileEx())
			 {
				 movie_full_path = fd->Get_Name();
			     movie_file_name = fd->Get_FilenameEx();
			 }
			  // dissects string into value and key for standard template library dictionary (=> std::map in File_Dialog class)
			  string help_str = movie_full_path;
			  int length = help_str.length();
			  help_str.erase(length - 4, length); //removes fileending e.g. ".avi"
			  // uses pathinfo and filename to create a folder, where the created file will be put in
			  CreateDirectory(help_str.c_str(),NULL);  
			  // sets pointer to selected video file => loads movie
			  vio->Get_Video_from_File(movie_full_path);
			  // fills textfield with pathinfo of loaded movie
			  Set_Text(pathinfo_handle,movie_full_path);
			  // fills textfield (labeled 'TO') with a loaded video's number of total frames
			  Transfer_Num_as_Text(to_handle,vio->Get_Total_Frames());
			  // writes movie frames as pics into directory
			  vio->Turn_Video_into_Pics(help_str);
			  
			}
			break;
		case IDM_EXIT:
			vio->Stop_Video();
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
		{
		 fd = new FileDialog(hWnd);
		 vio = new Video_OP();

		 
		 //PATH FOR WRITING DATA FILES // not used here
		     
			 DWORD x = GetCurrentDirectory(250,lpBuffer);
			 // not needed here 
			 vio->Create_Dest_Folder(lpBuffer);
		
		  // Static control as caption for path info textfield
		     path_caption_handle = CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC","PATHINFO",WS_CHILD | WS_VISIBLE,
		       12,10,100,30,hWnd,(HMENU) IDC_STATIC_PATH,GetModuleHandle(NULL),NULL);

		   // Textfield (Edit control) for pathinfo
		     pathinfo_handle = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD | WS_VISIBLE,
		       12,50,500,30,hWnd,(HMENU) IDC_EDIT_PATH,GetModuleHandle(NULL),NULL);

		   // Static control saying FROM
		     from_caption_handle = CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC","FROM",WS_CHILD | WS_VISIBLE,
		       12,90,50,30,hWnd,(HMENU) IDC_STATIC_START,GetModuleHandle(NULL),NULL);

		   // Static control saying TO
		      to_caption_handle = CreateWindowEx(WS_EX_CLIENTEDGE,"STATIC","TO",WS_CHILD | WS_VISIBLE,
		       12,130,50,30,hWnd,(HMENU) IDC_STATIC_STOP,GetModuleHandle(NULL),NULL);
		 
		   // Textfield to fill in frame where video shall start from
		      from_handle = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD | WS_VISIBLE,
		       72,90,50,30,hWnd,(HMENU) IDC_EDIT_START,GetModuleHandle(NULL),NULL);

		   // Textfield for telling program where to stop 
		      to_handle = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD | WS_VISIBLE,
		       72,130,50,30,hWnd,(HMENU) IDC_EDIT_STOP,GetModuleHandle(NULL),NULL);

		   // Button to start processes which were selected in the menu
		       button_handle = CreateWindowEx(WS_EX_CLIENTEDGE,"BUTTON","GO",WS_CHILD | WS_VISIBLE,
		       22,180,80,40,hWnd,(HMENU) IDC_BUTTON_DO_IT,GetModuleHandle(NULL),NULL);

          

			   // commoncontrol DLL needs to be loaded (needed for trackbar) 
			   //Attention: also requires <commctrl.h> and comctl32.lib
			   INITCOMMONCONTROLSEX icc;
			   icc.dwICC = ICC_BAR_CLASSES;
			   icc.dwSize = sizeof(icc);
		        bool z = InitCommonControlsEx(&icc);

           // Creates a trackbar 
		     track_bar_handle = CreateWindowEx(WS_EX_CLIENTEDGE,TRACKBAR_CLASS,"Trackbar",WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS | TBS_ENABLESELRANGE,
		       152,90,180,40,hWnd,(HMENU) IDC_TRACKBAR,GetModuleHandle(NULL),NULL);
          // Sets Range for Trackbar
			 SendMessage(track_bar_handle,TBM_SETRANGEMAX,0,25);

	

		
			 check_box_handles[0] = CreateWindowEx(WS_EX_CLIENTEDGE,"BUTTON","Blur",WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 152, 150,
				    150,16,hWnd,(HMENU) IDC_CKECK_BOX_BLUR, GetModuleHandle(NULL),NULL);

			 check_box_handles[1] = CreateWindowEx(WS_EX_CLIENTEDGE,"BUTTON","Erode",WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 152, 170,
				    150,16,hWnd,(HMENU) IDC_CKECK_BOX_ERODE, GetModuleHandle(NULL),NULL);

			 check_box_handles[2] = CreateWindowEx(WS_EX_CLIENTEDGE,"BUTTON","Dilate",WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 152, 190,
				    150,16,hWnd,(HMENU) IDC_CKECK_BOX_DILATE, GetModuleHandle(NULL),NULL);

			 check_box_handles[3] = CreateWindowEx(WS_EX_CLIENTEDGE,"BUTTON","Contour Threshold",WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 152, 210,
				    150,16,hWnd,(HMENU) IDC_CKECK_BOX_CONT_THRESH, GetModuleHandle(NULL),NULL);


			 // Textfield (Edit control) for trackbar position
		     trackbar_info_handle = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","",WS_CHILD | WS_VISIBLE,
		       350,90,50,40,hWnd,(HMENU) IDC_EDIT_TRACKBAR_INFO,GetModuleHandle(NULL),NULL);

           // Gets Menu Handle
		    menu_handle = GetMenu(hWnd);

			CheckMenuItem(menu_handle,IDM_PLAY_MOV,MF_CHECKED);
	
	        Set_Text(button_handle,"NO MOV");

		   // sets up a messages handler for modeless dialog (see [REF])
		  // dialog_handle = CreateDialog(GetModuleHandle(NULL),
			// MAKEINTRESOURCE(ID_DIA_LOADED_FILES),hWnd,Loaded_Files_Proc);
		 

		   
		}
		break;
    // Messages coming from the slider/scrollbar/trackbar (whatever it is called)
    case WM_HSCROLL:
		{
		LRESULT res = SendMessage(track_bar_handle,TBM_GETPOS,0,0);

		// check radiobutton (option button)

		if (BST_CHECKED == SendMessage(check_box_handles[0],BM_GETCHECK,0,0))
		    {
		      vio->Blur(int(res));
		    }
		else if (BST_CHECKED == SendMessage(check_box_handles[1],BM_GETCHECK,0,0))
		    {
     		  vio->Erode((int)res);
		    }
		else if (BST_CHECKED == SendMessage(check_box_handles[2],BM_GETCHECK,0,0))
		    {
		      vio->Dilate((int)res);
		    }
		else if (BST_CHECKED == SendMessage(check_box_handles[3],BM_GETCHECK,0,0))
		    {
              vio->Draw_Contours((int)res);
		    }
		
		  // fills textfield (labeled 'TO') with a loaded video's number of total frames
		   Transfer_Num_as_Text(trackbar_info_handle,(int)res);
		}
		break;
    // press left mouse button somewhere in the main window to uncheck all radiobuttons (optionbuttons)
	case WM_LBUTTONDOWN:
		{  

			vio->Reset_Manipulators();
			for (int i= 0; i < 5; i++)
			{
			   SendMessage(check_box_handles[i],BM_SETCHECK,BST_UNCHECKED,0);
			}

		}
		break;
/* // not needed
    case WM_KEYDOWN:
		{
		  	int x =0;
		  switch(wParam)
		  {
		  case VK_LEFT:
               x =Beep(100,200);
			  break;
		  case VK_RIGHT:
			  x = Beep(300,200);
			  break;
		  }
		}
		break;
*/
	case WM_DESTROY:
		vio->Stop_Video();
      
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



// Message Loop for Dialog
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// message handler for loaded files dialog  // not implemented in compiled program (see [REF]
/*
BOOL CALLBACK Loaded_Files_Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
    case WM_COMMAND:
		if (LOWORD(wParam) == IDC_CLOSE_LOADED_FILES ) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;

	}
	return FALSE;
}
*/

//ADDITIONAL GLOBAL FUNCTIONS

// displays char String in Window of selected handle
void Display_String(HWND handle,int where_x, int where_y, char* str)
{
	
  HDC hDC = GetDC(handle);
  TextOut(hDC,where_x,where_y,str,strlen(str));
  ReleaseDC(handle,hDC);
}


// returns number contained in a static or edit control
int Get_Number(HWND handle) 
 {
	 char buf[30];
	 int x = GetWindowText(handle,buf,strlen(buf));
	   if(strlen > 0)
       {
         return atoi(buf);
	   }
	   
        return 0;
 }
// transfer Integer to a static or edit control
 void Transfer_Num_as_Text(HWND handle, int number)
 {
	 
   char buf[30];
   sprintf(buf,"%d",(int)number);
   SendMessage((HWND)handle,(UINT) WM_SETTEXT,NULL,(LPARAM) buf);
 }

// transfers Text (i.e. char) to control 
 void Set_Text(HWND handle, char* str)
 {
   
   SendMessage((HWND)handle,(UINT) WM_SETTEXT,NULL,(LPARAM) str);
 
 }

 