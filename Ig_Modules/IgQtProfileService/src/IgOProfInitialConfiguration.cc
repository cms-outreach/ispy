//<<<<<< INCLUDES                                                       >>>>>>

#include "Ig_Modules/IgQtProfileService/src/IgOProfInitialConfiguration.h"
#include <qfile.h>
#include <qdom.h>
#include <qmessagebox.h>
#include <iostream>
#include "Ig_Modules/IgQtProfileService/src/IgOProfApplication.h"
#include "Ig_Modules/IgQtProfileService/src/IgOProfConfiguration.h"

//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>
/*
IgInitialConfiguration::IgInitialConfiguration(IgOProfApplication *parent, IgOProfConfiguration **config)
    :CanRestart(true)
{
    IgOProfConfiguration *local_config = new IgOProfConfiguration("~/.koprofilerrc");
    IgOProfConfiguration *global_config = new IgOProfConfiguration("/tmp/koprofilerrc");
       
    if(!local_config->IsConfigOnDisk)
    {
	local_config->write();
    }	
    
    if(!global_config->IsConfigOnDisk)
    {
	global_config = local_config;
	global_config->write();
    }
    
    if(global_config->User != local_config->User)
    {
	QMessageBox::warning(parent,"Koprofiler","Another user has locked oprofile deamon.Using his/her configuration","Ok");
	CanRestart = false;
	*config = global_config;
    }else if(global_config != local_config)
    {
	int choice = QMessageBox::warning(parent,"Koprofiler","Your local config and global config are different. Load new one and restart profiler?","Yes","No");
	if(choice == 0)
	{
	    global_config = local_config;
	    global_config->write();
	    *config = local_config;
	    *config->use();
	}else
	{
	}
    }
}
*/
