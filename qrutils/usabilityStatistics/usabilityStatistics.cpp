#include <QtCore/QDir>
#include <QtCore/QDate>
#include <QtCore/QDebug>
#include <QPointF>

#include "usabilityStatistics.h"

using namespace utils;

UsabilityStatistics* UsabilityStatistics::object = NULL;

QTextStream UsabilityStatistics::elementOnSceneCreationStream;
QTextStream UsabilityStatistics::errorReporterStream;
QTextStream UsabilityStatistics::totalTimeStream;
QTextStream UsabilityStatistics::menuElementUsingStream;
QTextStream UsabilityStatistics::mouseClickPositionStream;
QTextStream UsabilityStatistics::settingChangesStream;

int UsabilityStatistics::creationNumber;
int UsabilityStatistics::errorReporterNumber;
int UsabilityStatistics::menuElementUsingNumber;
int UsabilityStatistics::mouseClickPositionNumber;
int UsabilityStatistics::settingChangesNumber;

QString const elementCreationFileName = "/elementOnSceneCreation.txt";
QString const errorReporterFileName = "/errorReporter.txt";
QString const totalTimeFileName = "/totalTime.txt";
QString const menuElementUsingFileName = "/menuElementUsing.txt";
QString const mouseClickPositionFileName = "/mouseClickPosition.txt";
QString const settingChangesFileName = "/settingChanges.txt";

UsabilityStatistics::UsabilityStatistics()
{
	QDir dir(".");

	///create and open all files
	mElementOnSceneCreationFile.setFileName(dir.absolutePath() + elementCreationFileName);
	if (mElementOnSceneCreationFile.open(QFile::WriteOnly | QFile::Truncate)) {
		elementOnSceneCreationStream.setDevice(&mElementOnSceneCreationFile);
	}
	creationNumber = 1;

	mErrorReporterFile.setFileName(dir.absolutePath() + errorReporterFileName);
	if (mErrorReporterFile.open(QFile::WriteOnly | QFile::Truncate)) {
		errorReporterStream.setDevice(&mErrorReporterFile);
	}
	errorReporterNumber = 1;

	mTotalTimeFile.setFileName(dir.absolutePath() + totalTimeFileName);
	if (mTotalTimeFile.open(QFile::WriteOnly | QFile::Truncate)) {
		totalTimeStream.setDevice(&mTotalTimeFile);
	}

	mMenuElementUsingFile.setFileName(dir.absolutePath() + menuElementUsingFileName);
	if (mMenuElementUsingFile.open(QFile::WriteOnly | QFile::Truncate)) {
		menuElementUsingStream.setDevice(&mMenuElementUsingFile);
	}
	menuElementUsingNumber = 1;

	mMouseClickPositionFile.setFileName(dir.absolutePath() + mouseClickPositionFileName);
	if (mMouseClickPositionFile.open(QFile::WriteOnly | QFile::Truncate)) {
		mouseClickPositionStream.setDevice(&mMouseClickPositionFile);
	}
	mouseClickPositionNumber = 1;

	mSettingChangesFile.setFileName(dir.absolutePath() + settingChangesFileName);
	if (mSettingChangesFile.open(QFile::WriteOnly | QFile::Truncate)) {
		settingChangesStream.setDevice(&mSettingChangesFile);
	}
	settingChangesNumber = 1;
}

void UsabilityStatistics::reportCreationOfElements(const QString &editorName, const QString elementName)
{
	if (!mStatus)
		return;

	QDateTime now = QDateTime::currentDateTime();
	elementOnSceneCreationStream << creationNumber << " "
			<< editorName << " "
			<< elementName << " "
			<< now.toString() << "\n";
	creationNumber++;
}

void UsabilityStatistics::reportErrorsOfElements(const QString &type, const QString &editorName, const QString &elementName, const QString &message)
{
	if (!mStatus)
		return;

	QDateTime now = QDateTime::currentDateTime();
	errorReporterStream << errorReporterNumber << " "
			<< type << " "
			<< editorName << " "
			<< elementName << " "
			<< message << " "
			<< now.toString() << "\n";
	errorReporterNumber++;
}

void UsabilityStatistics::reportTotalTimeOfExec(const QString &totalTime, const int &exitCode)
{
	if (!mStatus)
		return;

	totalTimeStream << "TotalSessionTime: "
			<< totalTime << " msecs Exit code:"
			<< exitCode << "\n";
}

void UsabilityStatistics::reportMenuElementsUsing(const QString &elementName, const QString &status)
{
	if (!mStatus)
		return;

	QDateTime now = QDateTime::currentDateTime();
	QString const statusText = (status == "none") ? "" : status + " ";
	menuElementUsingStream << menuElementUsingNumber << " "
			<< elementName << " "
			<< statusText
			<< now.toString() << "\n";
	menuElementUsingNumber++;
}

void UsabilityStatistics::reportMouseClickPosition(const QPoint &pos)
{
	if (!mStatus)
		return;

	QDateTime now = QDateTime::currentDateTime();
	mouseClickPositionStream << mouseClickPositionNumber << " ("
			<< QString::number(pos.x()) << ", "
			<< QString::number(pos.y()) << ") "
			<< now.toString() << "\n";
	mouseClickPositionNumber++;
}

void UsabilityStatistics::reportSettingsChangesInfo(const QString &name, const QString &oldValue, const QString &newValue)
{
	if (!mStatus)
		return;

	QDateTime now = QDateTime::currentDateTime();
	settingChangesStream << settingChangesNumber << " "
			<< name << " "
			<< oldValue << " "
			<< newValue << " "
			<< now.toString() << "\n";
	settingChangesNumber++;
}

void UsabilityStatistics::setActualStatus(bool status)
{
	mStatus = status;
}

UsabilityStatistics *UsabilityStatistics::instance()
{
	if (object == NULL) {
		object = new UsabilityStatistics();
	}
	return object;
}

UsabilityStatistics::~UsabilityStatistics()
{
	mElementOnSceneCreationFile.close();
	mErrorReporterFile.close();
	mTotalTimeFile.close();
	mMenuElementUsingFile.close();
	mMouseClickPositionFile.close();
	mSettingChangesFile.close();

	if (!mStatus)
		return;

	QDir dir(".");
	QString now = QString::number(QDate::currentDate().year()) + "."
			+ QString::number(QDate::currentDate().month()) + "."
			+ QString::number(QDate::currentDate().day()) + "_"
			+ QString::number(QTime::currentTime().hour()) + "."
			+ QString::number(QTime::currentTime().minute()) + "."
			+ QString::number(QTime::currentTime().second()) + "."
			+ QString::number(QTime::currentTime().msec());

	QString const oldElementOnSceneCreationName = mElementOnSceneCreationFile.fileName();
	QString const oldErrorReporterName = mErrorReporterFile.fileName();
	QString const oldTotalTimeName = mTotalTimeFile.fileName();
	QString const oldMenuElementUsingName = mMenuElementUsingFile.fileName();
	QString const oldMouseClickPositionName = mMouseClickPositionFile.fileName();
	QString const oldSettingChangesName = mSettingChangesFile.fileName();

	QString const newDirName = now;
	QString const newFileElementOnSceneCreationName = newDirName + elementCreationFileName;
	QString const newFileErrorReporterName = newDirName + errorReporterFileName;
	QString const newFileTotalTimeName = newDirName + totalTimeFileName;
	QString const newFileMenuElementUsingName = newDirName + menuElementUsingFileName;
	QString const newFileMouseClickPositionName = newDirName + mouseClickPositionFileName;
	QString const newFileSettingChangesName = newDirName + settingChangesFileName;

	if (dir.cdUp()) {
		dir.cd("usabilityFiles");
		QString const dirAbsolutePathName = dir.absolutePath() + "/";
		QString const newElementOnSceneCreationName = dirAbsolutePathName + newFileElementOnSceneCreationName;
		QString const newErrorReporterName = dirAbsolutePathName + newFileErrorReporterName;
		QString const newTotalTimeName = dirAbsolutePathName + newFileTotalTimeName;
		QString const newMenuElementUsingName = dirAbsolutePathName + newFileMenuElementUsingName;
		QString const newMouseClickPositionName = dirAbsolutePathName + newFileMouseClickPositionName;
		QString const newSettingChangesName = dirAbsolutePathName + newFileSettingChangesName;

		dir.mkdir(newDirName);
		QFile::copy(oldElementOnSceneCreationName, newElementOnSceneCreationName);
		QFile::copy(oldErrorReporterName, newErrorReporterName);
		QFile::copy(oldTotalTimeName, newTotalTimeName);
		QFile::copy(oldMenuElementUsingName, newMenuElementUsingName);
		QFile::copy(oldMouseClickPositionName, newMouseClickPositionName);
		QFile::copy(oldSettingChangesName, newSettingChangesName);
	}
	//close all files and move to usabilbity files folder
}

void UsabilityStatistics::reportCreation(const QString &editorName, const QString elementName)
{
	instance()->reportCreationOfElements(editorName, elementName);
}

void UsabilityStatistics::reportErrors(const QString &type, const QString &editorName, const QString &elementName, const QString &message)
{
	instance()->reportErrorsOfElements(type, editorName, elementName, message);
}

void UsabilityStatistics::reportTotalTime(QString const &totalTime, int const &exitCode)
{
	instance()->reportTotalTimeOfExec(totalTime, exitCode);
}

void UsabilityStatistics::reportMenuElements(const QString &elementName, const QString &status)
{
	instance()->reportMenuElementsUsing(elementName, status);
}

void UsabilityStatistics::reportMouseClick(const QPoint &pos)
{
	instance()->reportMouseClickPosition(pos);
}

void UsabilityStatistics::reportSettingsChanges(const QString &name, const QVariant &oldValue, const QVariant &newValue)
{
	instance()->reportSettingsChangesInfo(name, oldValue.toString(), newValue.toString());
}

void UsabilityStatistics::setStatus(bool status)
{
	instance()->setActualStatus(status);
}
