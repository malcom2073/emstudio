/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2020 Michael Carpenter (malcom2073@gmail.com)                      *
 *                                                                                  *
 * This file is a part of EMStudio is licensed MIT as                               *
 * defined in LICENSE.md at the top level of this repository                        *
 ************************************************************************************/

#include <cassert>
#include <QApplication>

#include <QString>

#include "pluginmanager.h"
#include "emscore.h"



int main(int argc, char **argv)
{
	EMSCore::instance(argc,argv);
	EMSCore::instance().run();
	return EMSCore::instance().exec();
}
