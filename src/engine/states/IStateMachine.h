#pragma once
#include <functional>
#include "State.h"
#include "../EReturnCode.h"


namespace Atlas {	
	/// <summary>
	/// Interface for a game state machine. This is used to manage game states (<see cref="GameState"/>) and transition between them.
	/// </summary>
	class IStateMachine {
	public:
		
		/// <summary>
		/// Gets the next registered game state. If none is found or an error occurs, a null pointer is returned.
		/// </summary>
		/// <returns></returns>
		virtual GameState* getNextGameState() = 0;
				
		/// <summary>
		/// Gets the next registered game state <b>RELATIVE TO THE GIVEN INDEX</b>. If none is found or an error occurs, a null pointer is returned.
		/// </summary>
		/// <param name="fromIndex">From index.</param>
		/// <returns>A GameState pointer that represents the next game state <b>RELATIVE TO THE GIVEN INDEX</b></returns>
		virtual GameState* getNextGameState(int fromIndex) = 0;
		
		/// <summary>
		/// Gets the previous registered game state. If none is found or an error occurs, a null pointer is returned.
		/// </summary>
		/// <returns>A GameState pointer that represents the previous game state</returns>
		virtual GameState* getPreviousGameState() = 0;
				
		/// <summary>
		/// Gets the previous game state <b>RELATIVE TO THE GIVEN INDEX</b>. If none is found or an error occurs, a null pointer is returned.
		/// </summary>
		/// <param name="fromIndex">From index.</param>
		/// <returns>A GameState pointer that represents the previous game state <b>RELATIVE TO THE GIVEN INDEX</b></returns>
		virtual GameState* getPreviousGameState(int fromIndex) = 0;
		
		/// <summary>
		/// Gets the current game state. 
		/// </summary>
		/// <returns>A GameState pointer that represents the current game state</returns>
		virtual GameState* getCurrentGameState() = 0;
		
		/// <summary>
		/// Gets the registered game state at the specified index. If none is found or an error occurs, a null pointer is returned.
		/// </summary>
		/// <param name="index">The index.</param>
		/// <returns>A GameState pointer that represents the game state at the specified index</returns>
		virtual GameState* getGameState(int index) = 0;
		
		/// <summary>
		/// Determines whether or not a game state is valid.
		/// </summary>
		/// <param name="state">The state.</param>
		/// <returns>
		///   <c>true</c> if [is valid game state] [the specified state]; otherwise, <c>false</c>.
		/// </returns>
		virtual bool isValidGameState(GameState* state) = 0;
				
		/// <summary>
		/// Checks if the next game state exists and is valid.
		/// </summary>
		/// <returns>A boolean that represents whether or not the next game state exists and is valid</returns>
		virtual bool doesNextGameStateExists() = 0;
		
		/// <summary>
		/// Checks if the previous game state exists and is valid.
		/// </summary>
		/// <returns>A boolean that represents whether or not the previous game state exists and is valid</returns>
		virtual bool doesPreviousGameStateExists() = 0;
		
		/// <summary>
		/// Gets the total number of registered game states.
		/// </summary>
		/// <returns>A integer that represents the total number of registered game states</returns>
		virtual int getGameStateCount() = 0;
		
		/// <summary>
		/// Sets the next game state <b>RELATIVE TO THE CURRENT GAME STATE</b>.
		/// </summary>
		/// <param name="nextState">State of the next.</param>
		virtual void setNextGameState(GameState* nextState) = 0;		

		/// <summary>
		/// Sets the next game state <b>RELATIVE TO THE SPECIFIED INDEX</b>.
		/// </summary>
		/// <param name="nextState">State of the next.</param>
		/// <param name="index">The index.</param>
		virtual void setNextGameState(GameState* nextState, int index) = 0;
		
		/// <summary>
		/// Sets the previous game state to the specified game state <b>RELATIVE TO THE CURRENT GAME STATE</b>.
		/// </summary>
		/// <param name="previousState">State of the previous.</param>
		virtual void setPreviousGameState(GameState* previousState) = 0;
		
		/// <summary>
		/// Sets the previous game state <b>RELATIVE TO THE SPECIFIED INDEX</b>.
		/// </summary>
		/// <param name="previousState">State of the previous.</param>
		/// <param name="index">The index.</param>
		virtual void setPreviousGameState(GameState* previousState, int index) = 0;
		
		/// <summary>
		/// Goes to the next game state <b>RELATIVE TO THE CURRENT GAME STATE</b>.
		/// </summary>
		virtual void gotoNextGameState() = 0;
		
		/// <summary>
		/// Goes to the next gamestate <b>RELATIVE TO THE SPECIFIED INDEX</b>.
		/// </summary>
		/// <param name="fromIndex">From index.</param>
		virtual void gotoNextGameState(int fromIndex) = 0;
		
		/// <summary>
		/// Goes to the previous gamestate <b>RELATIVE TO THE CURRENT GAME STATE</b>.
		/// </summary>
		virtual void gotoPreviousGameState() = 0;
		
		/// <summary>
		/// Goes to the previous gamestate <b>RELATIVE TO THE SPECIFIED INDEX</b>.
		/// </summary>
		/// <param name="fromIndex">From index.</param>
		virtual void gotoPreviousGameState(int fromIndex) = 0;
		
		/// <summary>
		/// Goes to a game state that is stored at the specified index.
		/// </summary>
		/// <param name="index">The index of the game state.</param>
		virtual void gotoGameState(int index) = 0;
		
		/// <summary>
		/// Restarts the current game state. This is especially helpful when you want to restart or "refresh" the current game state. For clarification on what a 
		/// game state is, please see <see cref="State"/>
		/// </summary>
		virtual void restartCurrentGameState() = 0;
	};
}