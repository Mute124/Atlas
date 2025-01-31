#include "IStateMachine.h"

Atlas::GameState* Atlas::StateMachine::getNextGameState()
{
    return nullptr;
}

Atlas::GameState* Atlas::StateMachine::getNextGameState(int fromIndex)
{
    return nullptr;
}

Atlas::GameState* Atlas::StateMachine::getPreviousGameState()
{
    return nullptr;
}

Atlas::GameState* Atlas::StateMachine::getPreviousGameState(int fromIndex)
{
    return nullptr;
}

Atlas::GameState* Atlas::StateMachine::getCurrentGameState()
{
    return nullptr;
}

Atlas::GameState* Atlas::StateMachine::getGameState(int index)
{
    return nullptr;
}

bool Atlas::StateMachine::isValidGameState(GameState* state)
{
    return false;
}

bool Atlas::StateMachine::doesNextGameStateExists()
{
    return false;
}

bool Atlas::StateMachine::doesPreviousGameStateExists()
{
    return false;
}

int Atlas::StateMachine::getGameStateCount()
{
    return 0;
}

void Atlas::StateMachine::setNextGameState(GameState* nextState)
{
}

void Atlas::StateMachine::setNextGameState(GameState* nextState, int index)
{
}

void Atlas::StateMachine::setPreviousGameState(GameState* previousState)
{
}

void Atlas::StateMachine::setPreviousGameState(GameState* previousState, int index)
{
}

void Atlas::StateMachine::gotoNextGameState()
{
}

void Atlas::StateMachine::gotoNextGameState(int fromIndex)
{
}

void Atlas::StateMachine::gotoPreviousGameState()
{
}

void Atlas::StateMachine::gotoPreviousGameState(int fromIndex)
{
}

void Atlas::StateMachine::gotoGameState(int index)
{
}

void Atlas::StateMachine::restartCurrentGameState()
{
}
