// Add global vars here

class GameState {
public:
	const char* m_name;
	GameState(const char* name) {
		m_name = name;
	};

	virtual void run(); // override this
}

GameState states[5] {
	CalibrationState(),
  ConnectionState(),
	IntializationState(),
  PlayHideTimerState(),
	PlayPlayTimerState(),
	RankingState()
};

class CalibrationState: GameState {
public:
	CalibrationState(): GameState("connection") {}

  // Add Class Vars here

	void run()
  {
    // Add Run Code here
  }
}

class ConnectionState: GameState {
public:
	ConnectionState(): GameState("connection") {}

	void run()
  {
    
  }
}

class InitalizationState: GameState {
public:
	InitalizationState(): GameState("connection") {}

	void run()
  {
    
  }
}

class PlayHideState: GameState {
public:
	PlayHideState(): GameState("connection") {}

	void run()
  {
    
  }
}

class PlayPlayState: GameState {
public:
	PlayPlayState(): GameState("connection") {}

	void run()
  {
    
  }
}

class RankingState: GameState {
public:
	RankingState(): GameState("connection") {}

	void run()
  {
    
  }
}

void setup() {
	// maybe we should do the state initialization here?
}

void loop() {
  // this is heavily blocking, though each `run` command has its own task delays.
  // i don't think this should be a problem?
	for (const auto& state: states) {
		state.run(); // run each state's run function in sequence
	}

}
