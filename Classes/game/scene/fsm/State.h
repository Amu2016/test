#ifndef _State_H_
#define _State_H_

enum GlobalState{
	SEEK,
	FLEE,
	WANDER
};

template<class entity_type>
class State{
public:
	virtual void Enter(entity_type*) = 0;
	virtual void Execute(entity_type*) = 0;
	virtual void Exit(entity_type*) = 0;
	virtual ~State(){}
};

#endif _State_H_