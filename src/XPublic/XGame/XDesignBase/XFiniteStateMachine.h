/**
*  @file     XFiniteStatusMachine.h
*  @brief    有限状态机基类
*  Details.
*  三个基类：
* 状态机基类 状态基类 转换事件基类
*
*  @author   wangxinxiao
*  @email    wxx1035@163.com
*  @version  1.0.0.0
*  @date     2022/6/3
*
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*  2022/6/2 | 1.0.0.0  | wangxinxiao      | Create file
*-
*/

#ifndef _PUB_BASE_XFINITESTATEMACHINE_H
#define _PUB_BASE_XFINITESTATEMACHINE_H

#include <string.h>
#include <list>
#include <unordered_map>
#include <memory>

namespace XDESIGNPATTERBASE {

class XITransition;
class XIFSMBase;
class XIState;

typedef std::shared_ptr<XIFSMBase> XIFSMPtr;
typedef std::shared_ptr<XITransition> XITransitionPtr;
typedef std::shared_ptr<XIState> XIStatePtr;

class XIState
{
public:
    virtual void onEnter(XIStatePtr prev);
    virtual void onExit(XIStatePtr next);
    virtual void onUpdate(float deltaTime);
    
    void addTransition(XITransitionPtr transition);
    
    void setStateMachine(XIFSMPtr stateMachine);
    
    void setName(std::string name) { m_str_name = name; }
    std::string getName() { return m_str_name; }
    
    void setTag(int tag) { m_i_tag = tag; }
    int getTag() {return m_i_tag; }
    
    void setParent(XIFSMPtr parent) { m_FSMBase_parent_ptr = parent; }
    XIFSMPtr getParent() { return m_FSMBase_parent_ptr; }
    
    void setTimer(float timer) { m_f_timer = timer; }
    
    void doTransition(XITransitionPtr transition);
    
protected:
    float m_f_timer;
    std::string m_str_name;
    int m_i_tag;
    std::list<XITransitionPtr> m_listPtr_transition;
    XIFSMPtr m_FSMBase_parent_ptr { nullptr };
};

class XITransition
{
public:
    virtual bool onCheck();
    virtual bool onCompleteCallBack();
    
    void setName(std::string name) { m_str_name = name; }
    std::string getName() { return m_str_name; }
    
    void setFromState(XIStatePtr fromState) { m_state_fromState_ptr = fromState; }
    XIStatePtr getFromState() { return m_state_fromState_ptr; }
    
    void setToState(XIStatePtr toState) { m_state_toState_ptr = toState; }
    XIStatePtr getToState() { return m_state_toState_ptr; }
protected:
    std::string m_str_name;
    XIStatePtr m_state_fromState_ptr;
    XIStatePtr m_state_toState_ptr;
};

class XIFSMBase
{
public:
    virtual void onUpdate(float deltaTime);
    
    void addState(XIStatePtr state);
    void removeState(XIStatePtr state);
    
    XIStatePtr getStateWithTag(int tag);
    XIStatePtr getStateWithName(std::string name);
    
    void setCurState(XIStatePtr state) { m_state_curState_ptr = state; }
    void setDefaultState(XIStatePtr defaultState) { m_state_defaultState_ptr = defaultState; }
protected:
    XIStatePtr m_state_curState_ptr;
    XIStatePtr m_state_defaultState_ptr;
    std::unordered_map<std::string, XIStatePtr> m_mapStrStatePtr_states;
    std::unordered_map<int, XIStatePtr> m_mapIntStatePtr_states;
};

}

#endif /* _PUB_BASE_XFINITESTATEMACHINE_H */
