
////////////////////////////////////////////////////////////////////////////////////
//    
//   Внизу этого файла примеры (закомментированные) для консольного приложения
//
//   E-mail: dllBridge@gmail.com
// 
//    3 Jan 2024  (19:56)  
//   31 Dec 2024  (13:39)   
//   10 Feb 2022  (16:12)
//   Нужно сделать программное отключение программы по её номеру. Отправить запрос на отключение, через dllBridge.dll
//    7 Feb 2022  (16:16)  
//   24 Dec 2021  (14:52) 
//   18 Sep 2020  (12:14)
//


#ifdef     _MSC_VER
#define    _CRT_SECURE_NO_WARNINGS
#endif

#include    <stdio.h>
#include   <string.h>
#include   <locale.h>                                                             //  Здесь живёт setlocale()
#include  <windows.h>


////////////////////////////////////////////////////////////////////////////////////
struct WL_msg                                                                     //  
{

       int        nCondition,   //  1 - Всё хорошо (подключен) 
		              nPrint;   //  Вариант печати. Для консоли, Windows-окна или другое...
	   char   szComment1[81],
		      szComment2[81],
			  szComment3[81];          // "Connected as 1001"

} msg; 


HINSTANCE                hDll;                                         // if(hDll != 0) библиотека подключена.

char 	       szPathDLL[111], 
			  szPathBase[100] = "C:\\dllBridge",                                       //  Default folder	                  
			   szWLError[201];
			//szStatusNum[99];          // "Connected as 1001"

int             ProgramNumber = 1001,   // Вы можете здесь поменять этот номер до подключения к dllBridge.dll                    
                OldPrg_Number,          // или вызвать dllBridge_init(7564); 7564 - это новый номер. 
                      nPerson =  21,                                                           //      User number
              (*Transit)[100];
                 
              
//////////////////////////////
int*   (*__getAdrBOSS)(int n);
int    (*__DisConnect)(int n);


int    (*__ConsolePrinting)(int Y, int X, const char* str, int Colour);

int dllBridge(const char *sz); 


void      Printing_messages();   // Печатает на экран текущее состояние подключения. 


#ifdef __cplusplus               // g++  (C++) code + + + + + + + + + + + + + + + + +

namespace
{
   struct WLMain
   {
		
	   int  res;
		
	   ///////////////////////////////////////////////	                         Transit[3][17] = 317;
	   WLMain()                                     //                   printf("\nres = %d \n", res); 
	   {

		      res = dllBridge(szPathBase);	
             // Printing_messages(); 
	   }
	   
	   ///////////////////////////////////////////////
	  ~WLMain()                                     //  
	   {
		   if(res) 
		   { 
		       __DisConnect(ProgramNumber); 
			     hDll = 0; 
		         strcpy(msg.szComment1, "Disconnected from the library \"dllBridge.dll\"."); 
				 strcpy(msg.szComment2, "..."); 
				// Printing_messages(); 
		   }
	   }	   
	
   } Obj;
   
}  

#endif                   // #else        // C   code + + + + + + + + + + + + + + + + +



//  Принимает номер программы. С его помощью происходит подклюние и взаимодействие с другими программами.
//  Возвращает "0" (нуль), если не смогли подключиться. 
//  Возвращает "1", если успешно подключились.  
///////////////////////////////////////////////////////////////////////////////
int dllBridge_init(int numb)                                                 //
{

	ProgramNumber = numb; 
	int res = dllBridge("C:/dllBridge");                                     //         Путь, где лежит dllBridge.dll
    Printing_messages();
return res;    
}



///////////////////////////////////////////////////////////////////////////////////////
int dllBridge(const char *sz)                                                        //
{

	if(hDll) 
	{ 
	 __DisConnect(OldPrg_Number);                                                       //  To reconnect DLL. 
	   FreeLibrary(hDll); 
	}  
	else {                                  //  Этот код исполняется только 1 раз !  
		
	   setlocale(LC_ALL, "Rus");            //  The Cyrillic alphabet to a console application.
       if(msg.nPrint == 0) msg.nPrint = 1;  //  Печать сообщений в окно консоли с паузой 2 сек. (По умолчанию). 
	}
	
	char   sz1[201];
	strcpy(sz1, sz);       
    strcat(sz1, "\\dllBridge.dll");
	
	
	hDll = LoadLibrary(sz1);                                                         
    
    int res = -2;
    
    if(hDll == 0) 
	{ 
	   res  = -1; 
	   hDll = LoadLibrary("dllBridge.dll");
	} 

	if(hDll == 0)
	{ 
	   res = 0; 
	   goto L_02; 
	} 
	                                              OldPrg_Number = ProgramNumber ; 
	sprintf(msg.szComment2, "The number of this program is %d." , ProgramNumber);

  __getAdrBOSS = (int*(*)(int))GetProcAddress(hDll,   "__getAdrBOSS");  				

	Transit = (int(*)[100])__getAdrBOSS(ProgramNumber);		//   Second_Number);//		
     	   
	if(     Transit ==  0) { res = -2; goto L_02; }                             //  All connections are busy.
	if((int)Transit == -1) { res = -1; goto L_02; }      //  A program with this number is already connected.

  __ConsolePrinting = (int(*)(int, int, const char*, int))GetProcAddress(hDll,   "__ConsolePrinting");   

  __DisConnect =   (int (*)(int))GetProcAddress(hDll, "__DisConnect");	 

	strcpy(szPathDLL, sz1);
	
	  
	Transit[10][0] = 2;  
		
	
	memcpy(&Transit[10][1], "Console project", 15);  //  ! ! ! //memcpy(&Transit[10][1], szProjectInfo[1], 23);
	
	strcpy(szPathBase, sz);   //  szPathBase 
	strcpy (msg.szComment1, "Successfully connected to \"dllBridge.dll\"."); 
//	sprintf(msg.szComment2, "The number of this program is %d." , ProgramNumber);
	sprintf(msg.szComment3,                  "Connected as %d  ", ProgramNumber);
	msg.nCondition = 1;                                                                // 1 = everything is OK
return       (int)hDll ;	
L_02: FreeLibrary(hDll);
	               msg.nCondition = 0;
                   //strcpy (msg.szComment2, "...");
				 //sprintf_s(   szStatusNum, "dllBridge connection error.");
    switch(res)
	{
		
	    case   0:  strcpy (msg.szComment1, "Error! I can't load the library \"dllBridge.dll\".");     
			       sprintf(msg.szComment2, "The library should be here: %s\\dllBridge.dll", szPathBase);
			       break;				   
		case  -1:  strcpy (msg.szComment1, "Error! A program with this number is already connected."); break;				   			       
	    case  -2:  strcpy (msg.szComment1, "Error! All connections are busy.");				   
			       					       			       
	} 	return res;   		
}
