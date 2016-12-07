/*
* Copyright (C) 2016
* Computer Graphics Group, The Institute of Computer Graphics and Algorithms, TU Wien
* Written by Tobias Klein <tklein@cg.tuwien.ac.at>
* All rights reserved.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "GLWidget.h"

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QStatusBar>
#include <QVariant>


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void displayTotalGPUMemory(float size);
	void displayUsedGPUMemory(float size);
	void displayFPS(int fps);

	inline GLWidget *getGLWidget()
	{
		return m_glWidget;
	}

	

	static std::vector<float> departureDelays;
	static std::vector<float> arrivalDelays;

protected slots :

	void openFileAction();
	void closeAction();



private:

	// USER INTERFACE ELEMENTS

	Ui_MainWindow *m_Ui;


	// DATA 

	enum DataType
	{
		NETCDF
	};

	struct FileType
	{
		QString filename;
		DataType type;
	} m_FileType;

	GLWidget *m_glWidget;
	std::vector<std::vector<Atom> > m_animation;
	

};

#endif
