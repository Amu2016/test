#ifndef _MonsterState_H_
#define _MonsterState_H_

template<class entity_type>
class MonsterWander<entity_type> : public State<entity_type>{
public:
	static MonsterWander* getInstance(){
		static MonsterWander* wander = new MonsterWander();
		return wander;
	}

	virtual void Enter(entity_type* monster);
	virtual void Execute(entity_type* monster);
	virtual void Exit(entity_type* monster);
	virtual ~State(){}

private:
	MonsterWander(){}
	const MonsterWander operator=(const MonsterWander&);
};

#endif _MonsterState_H_