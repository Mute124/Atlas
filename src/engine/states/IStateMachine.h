#pragma once
#include <functional>
#include "State.h"
#include "../EReturnCode.h"


namespace Atlas {	
	class StateMachine {
	public:
		
		/// <summary>
		/// Gets the next registered game state. If none is found or an error occurs, a null pointer is returned.
		/// </summary>
		/// <returns></returns>
		virtual GameState* getNextGameState();
				
		/// <summary>
		/// Gets the next registered game state <b>RELATIVE TO THE GIVEN INDEX</b>. If none is found or an error occurs, a null pointer is returned.
		/// </summary>
		/// <param name="fromIndex">From index.</param>
		/// <returns>A GameState pointer that represents the next game state <b>RELATIVE TO THE GIVEN INDEX</b></returns>
		virtual GameState* getNextGameState(int fromIndex);
		
		/// <summary>
		/// Gets the previous registered game state. If none is found or an error occurs, a null pointer is returned.
		/// </summary>
		/// <returns>A GameState pointer that represents the previous game state</returns>
		virtual GameState* getPreviousGameState();
				
		/// <summary>
		/// Gets the previous game state <b>RELATIVE TO THE GIVEN INDEX</b>. If none is found or an error occurs, a null pointer is returned.
		/// </summary>
		/// <param name="fromIndex">From index.</param>
		/// <returns>A GameState pointer that represents the previous game state <b>RELATIVE TO THE GIVEN INDEX</b></returns>
		virtual GameState* getPreviousGameState(int fromIndex);
		
		/// <summary>
		/// Gets the current game state. 
		/// </summary>
		/// <returns>A GameState pointer that represents the current game state</returns>
		virtual GameState* getCurrentGameState();
		
		/// <summary>
		/// Gets the registered game state at the specified index. If none is found or an error occurs, a null pointer is returned.
		/// </summary>
		/// <param name="index">The index.</param>
		/// <returns>A GameState pointer that represents the game state at the specified index</returns>
		virtual GameState* getGameState(int index);
		
		/// <summary>
		/// Determines whether or not a game state is valid.
		/// </summary>
		/// <param name="state">The state.</param>
		/// <returns>
		///   <c>true</c> if [is valid game state] [the specified state]; otherwise, <c>false</c>.
		/// </returns>
		virtual bool isValidGameState(GameState* state);
				
		/// <summary>
		/// Checks if the next game state exists and is valid.
		/// </summary>
		/// <returns>A boolean that represents whether or not the next game state exists and is valid</returns>
		virtual bool doesNextGameStateExists();
		
		/// <summary>
		/// Checks if the previous game state exists and is valid.
		/// </summary>
		/// <returns>A boolean that represents whether or not the previous game state exists and is valid</returns>
		virtual bool doesPreviousGameStateExists();
		
		/// <summary>
		/// Gets the total number of registered game states.
		/// </summary>
		/// <returns>A integer that represents the total number of registered game states</returns>
		virtual int getGameStateCount();
		
		/// <summary>
		/// Sets the next game state <b>RELATIVE TO THE CURRENT GAME STATE</b>.
		/// </summary>
		/// <param name="nextState">State of the next.</param>
		virtual void setNextGameState(GameState* nextState);		

		/// <summary>
		/// Sets the next game state <b>RELATIVE TO THE SPECIFIED INDEX</b>.
		/// </summary>
		/// <param name="nextState">State of the next.</param>
		/// <param name="index">The index.</param>
		virtual void setNextGameState(GameState* nextState, int index);
		
		/// <summary>
		/// Sets the previous game state to the specified game state <b>RELATIVE TO THE CURRENT GAME STATE</b>.
		/// </summary>
		/// <param name="previousState">State of the previous.</param>
		virtual void setPreviousGameState(GameState* previousState);
		
		/// <summary>
		/// Sets the previous game state <b>RELATIVE TO THE SPECIFIED INDEX</b>.
		/// </summary>
		/// <param name="previousState">State of the previous.</param>
		/// <param name="index">The index.</param>
		virtual void setPreviousGameState(GameState* previousState, int index);
		
		/// <summary>
		/// Goes to the next game state <b>RELATIVE TO THE CURRENT GAME STATE</b>.
		/// </summary>
		virtual void gotoNextGameState();
		
		/// <summary>
		/// Goes to the next gamestate <b>RELATIVE TO THE SPECIFIED INDEX</b>.
		/// </summary>
		/// <param name="fromIndex">From index.</param>
		virtual void gotoNextGameState(int fromIndex);
		
		/// <summary>
		/// Goes to the previous gamestate <b>RELATIVE TO THE CURRENT GAME STATE</b>.
		/// </summary>
		virtual void gotoPreviousGameState();
		
		/// <summary>
		/// Goes to the previous gamestate <b>RELATIVE TO THE SPECIFIED INDEX</b>.
		/// </summary>
		/// <param name="fromIndex">From index.</param>
		virtual void gotoPreviousGameState(int fromIndex);
		
		/// <summary>
		/// Goes to a game state that is stored at the specified index.
		/// </summary>
		/// <param name="index">The index of the game state.</param>
		virtual void gotoGameState(int index);
		
		/// <summary>
		/// Restarts the current game state. This is especially helpful when you want to restart or "refresh" the current game state. For clarification on what a 
		/// game state is, please see <see cref="State"/>
		/// </summary>
		virtual void restartCurrentGameState();
	};
}