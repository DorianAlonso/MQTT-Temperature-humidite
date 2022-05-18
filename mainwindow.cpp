#include "mainwindow.h"
#include "subscriptionwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QtCore/QDateTime>
#include <QtWidgets/QMessageBox>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{



    ui->setupUi(this);
    QString user = "gps-temp@ttn";
    QString pass = "NNSXS.JGG2H7UUXNUWURYPT53NPJ6HN77SQP7RQYJ2LAA.OLT2MEDOGEGSLK3RUT73FR7E4EVVBK6KNZ7LINPBDQGXQOL3CDQQ";
    m_client = new QMqttClient(this);
    m_client->setHostname("eu1.cloud.thethings.network");
    m_client->setPort(1883);
    m_client->setUsername(user);
    m_client->setPassword(pass);
    m_client->connectToHost();




    connect(m_client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {




            QJsonDocument documentJSON = QJsonDocument::fromJson(message);
            QJsonObject objetJSON = documentJSON.object();
            QStringList listeCles;
            listeCles = objetJSON.keys();
            QJsonObject objetJSON2 = objetJSON[listeCles.at(3)].toObject();
            QStringList listeCles2;
            listeCles2 = objetJSON2.keys();
            QJsonObject objetJSON3 = objetJSON2[listeCles2.at(2)].toObject();
            QJsonArray jsonArray = objetJSON3["bytes"].toArray();
            int tab[11];
            char value;
            char chaine [13];
            for (int i=0;i<11;i++)
            {
                tab[i]=jsonArray.at(i).toInt();
                value = char(tab[i]);
                chaine[i]=value;
            }
            chaine[12]=0;

            char hum [7];
            for (int i=0;i<5;i++)
            {
               hum[i]=chaine[i];
            }
            hum[6]=0;

            int a=0;

            char temp [7];
            for (int i=6;i<11;i++)
            {
               temp[a]=chaine[i];
               a++;
            }
            temp[6]=0;

ui->label->setText(hum);
ui->label1->setText(temp);

QString fileName = "donnees.csv";
QFile file;
QTextStream flux;
QString sDate = QDateTime::currentDateTime().toString("dddd dd MMMM yyyy");
QString sHeure = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
file.setFileName(fileName);
file.open(QIODevice::WriteOnly| QFile::Text | QIODevice::Append);
if (!file.isOpen()) qDebug () << "Erreur ouverture du fichier en écriture" + fileName;
flux.setDevice(&file);
flux<<sDate<<'\t'<<";"<<sHeure<<";"<<'\t'<<temp<<'\t'<<";"<<hum<<"\n";
file.close();



    });




}

MainWindow::~MainWindow()
{
}

void MainWindow::on_Subscribe_clicked()
{
    QString topic ="v3/gps-temp@ttn/devices/eui-a8610a3434287a19/up";
    auto subscription = m_client->subscribe(topic,0);
    if (!subscription) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }
    ui->Subscribe->hide();
}


