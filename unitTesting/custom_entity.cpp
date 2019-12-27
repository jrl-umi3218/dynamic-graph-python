/* Copyright 2010-2019 LAAS, CNRS
 * Thomas Moulard.
 *
 */

#define ENABLE_RT_LOG

#include <sstream>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/exception-factory.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/pool.h>
#include <dynamic-graph/real-time-logger.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>
#include <dynamic-graph/command-bind.h>

namespace dynamicgraph {
class CustomEntity : public Entity {
 public:
  dynamicgraph::SignalPtr<double, int> m_sigdSIN;
  dynamicgraph::SignalTimeDependent<double, int> m_sigdTimeDepSOUT;

  static const std::string CLASS_NAME;
  virtual const std::string &getClassName() const { return CLASS_NAME; }
  CustomEntity(const std::string n)
      : Entity(n),
        m_sigdSIN(NULL, "CustomEntity(" + name + ")::input(double)::in_double"),
        m_sigdTimeDepSOUT(boost::bind(&CustomEntity::update, this, _1, _2), m_sigdSIN,
                          "CustomEntity(" + name + ")::input(double)::out_double")

  {
    addSignal();

    using namespace dynamicgraph::command;

    this->addCommand("act", makeCommandVoid0( *this, &CustomEntity::act,
          docCommandVoid0( "act on input signal")));
  }

  void addSignal() { signalRegistration(m_sigdSIN << m_sigdTimeDepSOUT); }

  void rmValidSignal() {
    signalDeregistration("in_double");
    signalDeregistration("out_double");
  }

  double &update(double &res, const int &inTime) {
    const double &aDouble = m_sigdSIN(inTime);
    res = aDouble;
    std::ostringstream oss;
    oss << "start update " << res;
    sendMsg(oss.str().c_str(), MSG_TYPE_ERROR);
    sendMsg("This is a message of level MSG_TYPE_DEBUG", MSG_TYPE_DEBUG, __FILE__, __LINE__);
    sendMsg("This is a message of level MSG_TYPE_INFO", MSG_TYPE_INFO, __FILE__, __LINE__);
    sendMsg("This is a message of level MSG_TYPE_WARNING", MSG_TYPE_WARNING, __FILE__, __LINE__);
    sendMsg("This is a message of level MSG_TYPE_ERROR", MSG_TYPE_ERROR, __FILE__, __LINE__);
    sendMsg("This is a message of level MSG_TYPE_DEBUG_STREAM", MSG_TYPE_DEBUG_STREAM, __FILE__, __LINE__);
    sendMsg("This is a message of level MSG_TYPE_INFO_STREAM", MSG_TYPE_INFO_STREAM, __FILE__, __LINE__);
    sendMsg("This is a message of level MSG_TYPE_WARNING_STREAM", MSG_TYPE_WARNING_STREAM, __FILE__, __LINE__);
    sendMsg("This is a message of level MSG_TYPE_ERROR_STREAM", MSG_TYPE_ERROR_STREAM, __FILE__, __LINE__);
    sendMsg("end update", MSG_TYPE_ERROR, __FILE__, __LINE__);
    return res;
  }

  void act() {
    m_sigdSIN.accessCopy();
  }
};
DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(CustomEntity, "CustomEntity");
}  // namespace dynamicgraph
