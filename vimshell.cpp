// vimshell.cpp : Defines the entry point for the application.
//

#include "vimshell.h"
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <shellapi.h>

bool cvtLPW2stdstring(std::string& s, const LPWSTR pw,
                      UINT codepage = CP_ACP)
{
    bool res = false;
    char* p = 0;
    int bsz;

    bsz = WideCharToMultiByte(codepage,
        0,
        pw,-1,
        0,0,
        0,0);
    if (bsz > 0) {
        p = new char[bsz];
        int rc = WideCharToMultiByte(codepage,
            0,
            pw,-1,
            p,bsz,
            0,0);
        if (rc != 0) {
            p[bsz-1] = 0;
            s = p;
            res = true;
        }
    }
    delete [] p;
    return res;
}

std::string FindAndReplace(std::string tInput, std::string tFind, std::string tReplace ) 
{ 
	size_t uPos = 0; 
	size_t uFindLen = tFind.length(); 
	size_t uReplaceLen = tReplace.length();

	if( uFindLen == 0 )
	{
		return tInput;
	}

	for( ;(uPos = tInput.find( tFind, uPos )) != std::string::npos; )
	{
		tInput.replace( uPos, uFindLen, tReplace );
		uPos += uReplaceLen;
	}

	return tInput;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	int count;
	//MessageBox(0, lpCmdLine, 0, 0);
	LPWSTR* strings = CommandLineToArgvW(GetCommandLineW(), &count);
	std::string currentDir;

	cvtLPW2stdstring(currentDir, strings[0]);

	// Cut off the filename
	while((currentDir.length() > 0) &&
		(currentDir[currentDir.length()-1] != '/') && 
		(currentDir[currentDir.length()-1] != '\\'))
	{
		currentDir = currentDir.substr(0, currentDir.length()-1);
	}
	if(currentDir.length() > 0 && currentDir[0] == '"')
		currentDir = currentDir.substr(1);
	
	std::string filename = currentDir + std::string("shell.txt");
	
	std::ifstream fileIn(filename.c_str());
	if(!fileIn)
	{
		MessageBox(0, "Please create a file called shell.txt in the same folder you put this new vimrun.exe", 0,0);
		return 1;
	}

	std::string argumentshellcommand;
	std::string silentshellcommand;
	std::string interactiveshellcommand;

	std::getline(fileIn, argumentshellcommand);
	std::getline(fileIn, silentshellcommand);
	std::getline(fileIn, interactiveshellcommand);

	std::string args = lpCmdLine;

	
	while(args.length() > 0 && args[0] == ' ')
		args = args.substr(1);

	bool execSilently = false;

	if(args.length() > 3 && 
			args[0] == '-' &&
			args[1] == 's' &&
			args[2] == ' ')
	{
		args = args.substr(3);
		execSilently = true;
	}

	size_t spacepos = args.find_first_of(" ");
	args = args.substr(spacepos+1);
	spacepos = args.find_first_of(" ");
	args = args.substr(spacepos+1);

	std::string cmd;

	if(spacepos == std::string::npos)
		args = "";

	std::string argcmd = execSilently ? silentshellcommand : argumentshellcommand;

	if(args.length() == 0)
	{
		cmd = interactiveshellcommand;
	}
	else
	{
		std::string quotedPH = "#QQQQ#";
		if(argcmd.find(quotedPH) != std::string::npos)
		{
			args = FindAndReplace(args,"\"", "\\\"");
			args = FindAndReplace(args, "\\", "\\\\");
			cmd = FindAndReplace(argcmd, quotedPH, args);
		}
		else
		{
			cmd = argcmd + " " + args;
		}
	}
	//MessageBox(0,cmd.c_str(), 0,0);
	// I know, I know, baaaaad win16 function, unfortunately I could not get
	// CreateProcess working as I wanted it (actually showing me the window)
	// and also if I used CreateProcess, I would have to parse the program name
	// out of the shell.txt file to know where the parameters begint etc, 
	// which is kinda difficult and not pleasant at all.
	WinExec(cmd.c_str(), SW_SHOW);
	return 0;
}