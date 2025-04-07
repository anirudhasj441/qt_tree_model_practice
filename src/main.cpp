/// @file: main.cpp
///
/// (C)2025 Coding Devil.

#include "gui/frm_main.h"

#include <QApplication>

#include <iostream>

//////////////////////////////////////////////////////////////////////////////

/// main() - the once and only.
///
/// @param[in] 
///     Number of commandline arguments passed in.
/// @param[in]
///     Array of char strings containing individual arguments.
int
main( int argc, char** argv ) {
    std::cout << "New project tree_model_demo" << std::endl;
    
    QApplication app( argc, argv );
    MainForm mf;
    
    mf.show();
    
    return app.exec();
}

