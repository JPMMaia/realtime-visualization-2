#include "Streamserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include "mainwindow.h"
#include "CUDAEngine/KDE2D.cuh"

#include <QSslConfiguration>
#include <QSslCertificate>
#include <QSslKey>
#include <math.h>
#include <iostream>
#include <QtCore/QDebug>
#include <QFileInfo>


QT_USE_NAMESPACE

StreamServer::StreamServer(quint16 port, bool debug, QWidget &widget, double pixelRatio, QObject *parent) :
	QObject(parent),
	m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Marion"), QWebSocketServer::NonSecureMode, this)),
	m_clients(),
	m_debug(debug),
	widget(widget),
	pixelRatio(pixelRatio)
{
	if (m_pWebSocketServer->listen(QHostAddress::Any, port))
	{
		if (m_debug)
			qDebug() << "Streamserver listening on port" << port;
		connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &StreamServer::onNewConnection);
		connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &StreamServer::closed);
		connect(m_pWebSocketServer, &QWebSocketServer::sslErrors, this, &StreamServer::onSslErrors);
	}
}

StreamServer::~StreamServer()
{
	m_pWebSocketServer->close();
	qDeleteAll(m_clients.begin(), m_clients.end());
}

void StreamServer::onSslErrors(const QList<QSslError> &errors)
{
	foreach(QSslError error, errors)
	{
		qDebug() << "SSL ERROR: " << error.errorString();
	}
}

void StreamServer::onNewConnection()
{
	QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

	connect(pSocket, &QWebSocket::textMessageReceived, this, &StreamServer::processTextMessage);
	connect(pSocket, &QWebSocket::binaryMessageReceived, this, &StreamServer::processBinaryMessage);
	connect(pSocket, &QWebSocket::disconnected, this, &StreamServer::socketDisconnected);

	m_clients << pSocket;

	//if (m_debug)
	std::cout << "new client" << std::endl;
		qDebug() << "new client!" << pSocket->resourceName();
}

void StreamServer::processTextMessage(QString message)
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	if (m_debug)
		qDebug() << "Message received:" << message;
	if (pClient) {

		if (message == "req_image") {
			createKDE();
			sendKDE(pClient);
		}

		else if (message.startsWith("KDE")) {
			QStringList chunks(message.split(" "));
			bool isok = false;
			minX = chunks.at(1).toLong(&isok);
			if (!isok)
				qDebug() << "Extracting minX failed";
			maxX = chunks.at(2).toLong(&isok);
			if (!isok)
				qDebug() << "Extracting maxX failed";
			minY = chunks.at(3).toLong(&isok);
			if (!isok)
				qDebug() << "Extracting minY failed";
			maxY = chunks.at(4).toLong(&isok);
			if (!isok)
				qDebug() << "Extracting maxY failed";
		}

	}
}

void StreamServer::sendKDE(QWebSocket *client) {
	if (0 == kde_image.size()) return;
	QByteArray ba;
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	buffer.write(QByteArray((const char*)kde_image.constData(), sizeof(float)*numBins*numBins));
	buffer.write(QByteArray((const char*)&numBins, sizeof(int)));
	int maxBin_i = int(::roundf(maxBin));
	buffer.write(QByteArray((const char*)&maxBin_i, sizeof(int)));
	buffer.close();
	
	if (ba != previousArray || true)
	{
		client->sendBinaryMessage(ba);
	}

	previousArray = QByteArray(ba);


}

extern "C"
float KDEEstimator2D(const float* x_arr, const float* y_arr, size_t data_num, float epsilon, float minX, float maxX, float minY, float maxY, float* kde_image, size_t numBins);

#include <QElapsedTimer>

void StreamServer::createKDE() {
	//todo: port to cuda
	const std::vector<float>& depart =  (MainWindow::departureDelays);
	const std::vector<float>& arr = (MainWindow::arrivalDelays);
	
	qDebug("size: %d", depart.size());
	kde_image.fill(0.0f, numBins*numBins);

	float minDep = float(minX);
	float maxDep = float(maxX);
	float rangeDep = maxDep - minDep;
	float minArr = float(minY);
	float maxArr = float(maxY);
	float rangeArr = maxArr - minArr;
	float epsilon_local = epsilon*std::max<float>(rangeDep / float(numBins - 1), rangeArr / float(numBins - 1));

	QElapsedTimer timer;
	timer.start();

	//maxBin = KDEEstimator2D(depart.data(), arr.data(), depart.size(), epsilon_local, minDep, maxDep, minArr, maxArr, kde_image.data(), numBins);
	maxBin = CallKDE2D(depart.data(), arr.data(), depart.size(), epsilon_local, minDep, maxDep, minArr, maxArr, kde_image.data(), numBins);

	qDebug("kde in %d msec", timer.elapsed());
}

void StreamServer::processBinaryMessage(QByteArray message)
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	if (m_debug)
		qDebug() << "Binary Message received:" << message;
	if (pClient) {
		pClient->sendBinaryMessage(message);
	}
}

void StreamServer::socketDisconnected()
{
	QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
	if (m_debug)
		qDebug() << "socketDisconnected:" << pClient;
	if (pClient) {
		m_clients.removeAll(pClient);
		pClient->deleteLater();
	}
}

void StreamServer::sendImage(QWebSocket *client, const char *format, int quality)
{
	GLWidget *canvas = dynamic_cast<MainWindow*> (&widget)->getGLWidget();

	QImage image = canvas->getImage();


	QByteArray ba;
	QBuffer buffer(&ba);
	buffer.open(QIODevice::WriteOnly);
	image.save(&buffer, format, quality);

	if (ba != previousArray || true)
	{
		client->sendBinaryMessage(ba);
	}

	previousArray = QByteArray(ba);
}
