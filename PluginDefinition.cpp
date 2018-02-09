//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"

//
// The plugin data that Notepad++ needs
//
FuncItem funcItem[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppData;


ShortcutKey *skHello;


//tool function declaration
char* get_sub_str(char *str, int start, int len);


//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInit(HANDLE hModule)
{
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInit()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    setCommand(0, TEXT("Hello Notepad++"), hello, NULL, false);
	skHello = new ShortcutKey;
	ShortcutKey sk;
	skHello->_isAlt = true;
	skHello->_isCtrl = false;
	skHello->_isShift = false;
	skHello->_key = 71;//G
    setCommand(1, TEXT("Hello (with dialog)"), helloDlg, skHello, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
	delete(skHello);
}


//
// This function help you to initialize your plugin commands
//
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit) 
{
    if (index >= nbFunc)
        return false;

    if (!pFunc)
        return false;

    lstrcpy(funcItem[index]._itemName, cmdName);
    funcItem[index]._pFunc = pFunc;
    funcItem[index]._init2Check = check0nInit;
    funcItem[index]._pShKey = sk;

    return true;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
void hello()
{
    // Open a new document
    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);

    // Get the current scintilla
    int which = -1;
    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
    if (which == -1)
        return;
    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;

    // Say hello now :
    // Scintilla control has no Unicode mode, so we use (char *) here
    ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"Hello, Notepad++!");
}

void helloDlg()
{
    //::MessageBox(NULL, TEXT("Hello, Notepad++!"), TEXT("Notepad++ Plugin Template"), MB_OK);
	//SendMessage(nppData._scintillaMainHandle, SCI_INDICSETFORE, 0, 0x00FF00); // green #00d000
	//SendMessage(nppData._scintillaMainHandle, SCI_INDICSETALPHA, 0, 100);
	//SendMessage(nppData._scintillaMainHandle, SCI_INDICSETSTYLE,0, INDIC_ROUNDBOX); 
	//SendMessage(nppData._scintillaMainHandle, SCI_SETINDICATORCURRENT, 0/*INDICATOR_CURRENT*/, 0);
	//SendMessage(nppData._scintillaMainHandle, SCI_INDICATORFILLRANGE,5, 7);
	NPPM_GETCURRENTLINE;
	NPPM_GETCURRENTCOLUMN;
	NPPM_GETCURRENTWORD;
	char cCurWord[MAX_PATH] = { 0 };
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTWORD, MAX_PATH, (LPARAM)cCurWord);
	::SendMessage(nppData._scintillaMainHandle, SCI_GETSELTEXT, 0, (LPARAM)cCurWord);
	//Sci_TextToFind scitexttofind;
	//int nPositon = ::SendMessage(nppData._scintillaMainHandle, SCI_SEARCHINTARGET, MAX_PATH, (LPARAM)szWord);
	Sci_TextToFind *ft = new Sci_TextToFind();
	Sci_CharacterRange cr;
	cr.cpMin = 0;
	cr.cpMax = ::SendMessage(nppData._scintillaMainHandle, SCI_GETLENGTH, 0, 0);//文档末尾
	ft->chrg = cr;
	ft->lpstrText = cCurWord;
	//strcpy(ft->lpstrText, szWord);

	int nTotalLines = ::SendMessage(nppData._scintillaMainHandle, SCI_GETLINECOUNT, 0, 0);

	int nPositon = ::SendMessage(nppData._scintillaMainHandle, SCI_FINDTEXT, SCFIND_WHOLEWORD | SCFIND_MATCHCASE, (LPARAM)ft);//循环搜索
	int nLine = 0;
	nLine = ::SendMessage(nppData._scintillaMainHandle, SCI_LINEFROMPOSITION, (WPARAM)nPositon, 0);//根据位置获取行数
	//关键词行数
	int nKeywordLine = nLine;

	while (1)
	{
		//::MessageBox(NULL, TEXT("Hello, Notepad++!"), TEXT("Notepad++ Plugin Template"), MB_OK);
		char cNextWord[MAX_PATH] = { 0 }, cNextLineWord[MAX_PATH] = { 0 };
		nPositon = ::SendMessage(nppData._scintillaMainHandle, SCI_GETLINE, nLine, (LPARAM)cNextWord);//第三行的所有文字包括\r\n
		nPositon = ::SendMessage(nppData._scintillaMainHandle, SCI_GETLINE, nLine + 1, (LPARAM)cNextLineWord);//第三行的所有文字包括\r\n
		if (!strncmp(cNextLineWord, "{", 1)
			&& !strcmp(cCurWord, get_sub_str(cNextWord, strlen(cNextWord) - 1 - strlen(cCurWord), strlen(cCurWord)))
			//&& (strlen(cCurWord) >= strlen(cNextWord) - 2)
			//&& !strncmp(cNextWord, cCurWord, strlen(cNextWord) - 2)//TODO
			)
		{
			::SendMessage(nppData._scintillaMainHandle, SCI_GOTOLINE, (WPARAM)nLine, 0);
			break;
		}
		nLine++;
		if (nLine == nTotalLines)
		{
			nLine = 0;
		}
		else if (nLine == nKeywordLine)
		{
			break;
		}
	}
	//int curLine = ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTLINE, 0, 0);
	NPPM_GETCURRENTLINE;
	NPPM_GETCURRENTCOLUMN;
	SCI_GOTOPOS;

	//SCI_LINEFROMPOSITION(int pos) → int
	//This message returns the line that contains the position pos in the document.
	//The return value is 0 if pos <= 0. The return value is the last line if pos is beyond the end of the document.
	
	//SCI_FINDTEXT(int searchFlags, Sci_TextToFind *ft) → position
	//SendMessage(nppData._scintillaMainHandle, SCI_GETFIRSTVISIBLELINE, 0, 0);
}


char* get_sub_str(char *str, int start, int len)
{
	char* res = (char*)malloc(len + 1);
	res[len] = 0;
	memcpy(res, str + start - 1, len);
	return res;
}
