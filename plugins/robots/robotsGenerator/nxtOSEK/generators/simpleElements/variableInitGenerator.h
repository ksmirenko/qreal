#include "../../../../../../qrkernel/ids.h"
#include "abstractsimpleelementgenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class VariableInitGenerator: public AbstractSimpleElementGenerator {
public:
	VariableInitGenerator();

protected:
	virtual QList<SmartLine> convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);
};
}
}
