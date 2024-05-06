#ifndef EXECUTIONPAGE_H
#define EXECUTIONPAGE_H

#ifndef Gui_h
  #include "Gui/Gui.h"
#endif

class ExecutionPage{
    private: 
        Gui* master;
    
    public:
        ExecutionPage();

        void showExecutingPage();
};

#endif; //EXECUTIONPAGE_H