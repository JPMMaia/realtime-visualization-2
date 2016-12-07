/*
* Copyright (C) 2016
* Computer Graphics Group, The Institute of Computer Graphics and Algorithms, TU Wien
* Written by Tobias Klein <tklein@cg.tuwien.ac.at>
* All rights reserved.
*/

#include "MainWindow.h"

#include <QFileDialog>
#include <qmessagebox.h>
#include <QPainter>
#include <QXmlStreamReader>
#include <QDomDocument>

#include "PdbLoader.h"
#include "NetCDFLoader.h"
#include <fstream>
#include <string>
#include <iostream>


std::vector<float> MainWindow::departureDelays;
std::vector<float> MainWindow::arrivalDelays;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_Ui = new Ui_MainWindow();
	m_Ui->setupUi(this);
	QLayout *layout = m_Ui->controls->layout();
	layout->setAlignment(Qt::AlignTop);
	m_Ui->controls->setLayout(layout);
		

	m_glWidget = new GLWidget(this, this);
	m_Ui->glLayout->addWidget(m_glWidget);
	

	connect(m_Ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFileAction()));
	connect(m_Ui->actionClose, SIGNAL(triggered()), this, SLOT(closeAction()));

	m_Ui->memSizeLCD->setPalette(Qt::darkGreen);
	m_Ui->usedMemLCD->setPalette(Qt::darkGreen);
	m_Ui->fpsLCD->setPalette(Qt::darkGreen);

}

MainWindow::~MainWindow()
{
}


//-------------------------------------------------------------------------------------------------
// Slots
//-------------------------------------------------------------------------------------------------

void MainWindow::openFileAction()
{
	//need to rewrite whole thing, just keeping it here for now.

	QString filename = QFileDialog::getOpenFileName(this, "Data File", 0);
	bool success = false;
	if (!filename.isEmpty())
	{

		std::ifstream in((std::string) filename.toStdString(), std::ifstream::binary | std::ios::ate);
		if (in) {
			// get length of file:
		//	in.seekg(0, in.end);
			int length = in.tellg();

			in.clear();
			in.seekg(0, std::ios::beg);

		

			int oneArraySize = (length / sizeof(int)) / 2; //byte to int32
			MainWindow::departureDelays.reserve(oneArraySize);
			MainWindow::arrivalDelays.reserve(oneArraySize);
			int upto = 500; //in order to reduce data set (otherwise it takes a long time).
			int count = 0;
			while (!in.eof() && count <upto) {
				int tmp;
				in.read((char*)&tmp, sizeof(int));
				if(!in.eof()){ //reading to end of file, possibly eofbit isn't set at the loop end.
				MainWindow::departureDelays.push_back(tmp);}

				
				in.read((char*)&tmp, sizeof(int));
				if (!in.eof()) {
					MainWindow::arrivalDelays.push_back(tmp);
				}	
				count += 1;
			}

			success = true;

		}
		in.close();
	}

	// status message
	if (success)
	{

		m_Ui->labelTop->setText("File LOADED [" + filename + "]");
	}
	else
	{
		m_Ui->labelTop->setText("ERROR loading file " + filename + "!");

	}

}




void MainWindow::closeAction()
{
	close();
}





void MainWindow::displayTotalGPUMemory(float size)
{
	m_Ui->memSizeLCD->display(size);
}
void MainWindow::displayUsedGPUMemory(float size)
{
	m_Ui->usedMemLCD->display(size);
}

void MainWindow::displayFPS(int fps)
{
	m_Ui->fpsLCD->display(fps);
}