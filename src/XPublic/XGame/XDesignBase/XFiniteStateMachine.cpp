#include "XFiniteStateMachine.h"

#include "iostream"

using namespace XDESIGNPATTERBASE;
using namespace std;


void XIState::onEnter(XIStatePtr prev) {
    cout << "State::onEnter" << endl;
}


void XIState::onExit(XIStatePtr next) {
    cout << "State::onExit" << endl;
}


void XIState::onUpdate(float deltaTime) {
    for (list<XITransitionPtr>::iterator iter = m_listPtr_transition.begin(); iter != m_listPtr_transition.end(); iter ++)
    {
        if ( (*iter)->onCheck() )
        {
            if ( (*iter)->onCompleteCallBack() )
            {
                doTransition(*iter);
            }
        }
    }
}


void XIState::addTransition(XITransitionPtr transition) {
    m_listPtr_transition.push_back(transition);
}


void XIState::setStateMachine(XIFSMPtr stateMachine) {
    m_FSMBase_parent_ptr = stateMachine;
}


void XIState::doTransition(XITransitionPtr transition) {
    transition->getFromState()->onExit(transition->getToState());
    m_FSMBase_parent_ptr->setCurState(transition->getToState());
    transition->getToState()->onEnter(transition->getFromState());
}



bool XITransition::onCheck() {
    cout << "Transition::onCheck" << endl;
    return true;
}


bool XITransition::onCompleteCallBack() {
    cout << "Transition::OnCompleteCallBack" << endl;
    return true;
}



void XIFSMBase::addState(XIStatePtr state) {
    state->setParent(std::shared_ptr<XIFSMBase>(this));
    m_mapStrStatePtr_states[state->getName()] = state;
    if (state->getTag() != -1)
    {
        m_mapIntStatePtr_states[state->getTag()] = state;
    }
}


void XIFSMBase::removeState(XIStatePtr state) {
    if (state->getTag() != -1)
    {
        m_mapIntStatePtr_states.erase(state->getTag());
    }
    m_mapStrStatePtr_states.erase(state->getName());
}


XIStatePtr XIFSMBase::getStateWithTag(int tag) {
    return m_mapIntStatePtr_states[tag];
}


XIStatePtr XIFSMBase::getStateWithName(string name) {
    return m_mapStrStatePtr_states[name];
}


void XIFSMBase::onUpdate(float deltaTime) { 
    m_state_curState_ptr->onUpdate(deltaTime);
}
