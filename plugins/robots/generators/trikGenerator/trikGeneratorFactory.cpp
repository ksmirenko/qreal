#include "trikGeneratorFactory.h"
#include <generatorBase/converters/regexpMultiConverter.h>
#include "converters/engineV4PortConverter.h"
#include "converters/engineV6PortConverter.h"
#include "converters/encoderV4PortConverter.h"
#include "converters/encoderV6PortConverter.h"
#include "simpleGenerators/detectLineGenerator.h"
#include "simpleGenerators/initCameraGenerator.h"
#include "simpleGenerators/ledGenerator.h"
#include "simpleGenerators/lineDetectorToVariableGenerator.h"
#include "simpleGenerators/playToneGenerator.h"
#include "simpleGenerators/sadSmileGenerator.h"
#include "simpleGenerators/sayGenerator.h"
#include "simpleGenerators/setBackgroundGenerator.h"
#include "simpleGenerators/smileGenerator.h"
#include "simpleGenerators/systemGenerator.h"
#include "simpleGenerators/trikEnginesGenerator.h"
#include "simpleGenerators/trikEnginesStopGenerator.h"
#include "simpleGenerators/waitForButtonGenerator.h"
#include "simpleGenerators/trikNullificationEncoderGenerator.h"
#include "parts/trikVariables.h"

using namespace trik;
using namespace trik::simple;
using namespace generatorBase;
using namespace generatorBase::simple;

TrikGeneratorFactory::TrikGeneratorFactory(qrRepo::RepoApi const &repo
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
	: GeneratorFactoryBase(repo, errorReporter, robotModelManager)
{
}

TrikGeneratorFactory::~TrikGeneratorFactory()
{
}

AbstractSimpleGenerator *TrikGeneratorFactory::simpleGenerator(qReal::Id const &id
		, GeneratorCustomizer &customizer)
{
	QString const elementType = id.element();
	if (elementType.contains("EnginesForward") || elementType.contains("EnginesBackward")) {
		return new TrikEnginesGenerator(mRepo, customizer, id, elementType, this);
	} else if (elementType.contains("EnginesStop")) {
		return new TrikEnginesStopGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("NullificationEncoder")) {
		return new TrikNullificationEncoderGenerator(mRepo, customizer, id, this);
	} else if (elementType.contains("PlayTone")) {
		return new PlayToneGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Smile") {
		return new SmileGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SadSmile") {
		return new SadSmileGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Say") {
		return new SayGenerator(mRepo, customizer, id, this);
	} else if (elementType == "SetBackground") {
		return new SetBackgroundGenerator(mRepo, customizer, id, this);
	} else if (elementType == "System") {
		return new SystemGenerator(mRepo, customizer, id, this);
	} else if (elementType == "Led") {
		return new LedGenerator(mRepo, customizer, id, this);
	} else if (elementType == "DetectLine") {
		return new DetectLineGenerator(mRepo, customizer, id, this);
	} else if (elementType == "InitCamera") {
		return new InitCameraGenerator(mRepo, customizer, id, this);
	} else if (elementType == "LineDetectorToVariable") {
		return new LineDetectorToVariableGenerator(mRepo, customizer, id, this);
	} else if (elementType == "WaitForEnter") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForEnter.t", this);
	} else if (elementType == "WaitForLeft") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForLeft.t", this);
	} else if (elementType == "WaitForRight") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForRight.t", this);
	} else if (elementType == "WaitForUp") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForUp.t", this);
	} else if (elementType == "WaitForDown") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForDown.t", this);
	} else if (elementType == "WaitForPower") {
		return new WaitForButtonGenerator(mRepo, customizer, id, "buttons/waitForPower.t", this);
	}

	return GeneratorFactoryBase::simpleGenerator(id, customizer);
}

QString TrikGeneratorFactory::pathToTemplates() const
{
	return ":/trik/templates";
}

Binding::MultiConverterInterface *TrikGeneratorFactory::enginesConverter() const
{
	return new generatorBase::converters::RegexpMultiConverter(motorPortSplitRegexp(), motorPortConverter());
}

Binding::MultiConverterInterface *TrikGeneratorFactory::encodersConverter() const
{
	return new generatorBase::converters::RegexpMultiConverter(motorPortSplitRegexp(), encoderPortConverter());
}

Binding::ConverterInterface *TrikGeneratorFactory::inputPortConverter() const
{
	return new Binding::EmptyConverter;
}

Binding::ConverterInterface *TrikGeneratorFactory::outputPortConverter() const
{
	return encoderPortConverter();
}

void TrikGeneratorFactory::initVariables()
{
	mVariables = new parts::TrikVariables(pathToTemplates(), mRobotModelManager.model());
}

Binding::ConverterInterface *TrikGeneratorFactory::motorPortConverter() const
{
	if (mRobotModelManager.model().name().contains("V4")) {
		return new converters::PowerV4MotorPortConverter;
	} else if (mRobotModelManager.model().name().contains("V6")) {
		return new converters::PowerV6MotorPortConverter;
	}

	/// @todo: Inconsistent scenario
	return new converters::PowerV4MotorPortConverter;
}

Binding::ConverterInterface *TrikGeneratorFactory::encoderPortConverter() const
{
	if (mRobotModelManager.model().name().contains("V4")) {
		return new converters::EncoderV4PortConverter;
	} else if (mRobotModelManager.model().name().contains("V6")) {
		return new converters::EncoderV6PortConverter;
	}
}

QString TrikGeneratorFactory::motorPortSplitRegexp() const
{
	if (mRobotModelManager.model().name().contains("V4")) {
		return converters::PowerV4MotorPortConverter::splitRegexp();
	} else if (mRobotModelManager.model().name().contains("V6")) {
		return converters::PowerV6MotorPortConverter::splitRegexp();
	}

	/// @todo: Inconsistent scenario
	return converters::PowerV4MotorPortConverter::splitRegexp();
}
