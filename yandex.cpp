#include <iostream>
#include <list>
#include <vector>
using namespace std;

class Device {
public:
	virtual ~Device() {
	}
};

class SmartSocket: public Device {
	bool on;
public:
	SmartSocket() {
		this->on = true;
	}
	void turnOn() {
		this->on = true;
	}
	void turnOff() {
		this->on = false;
	}
	void toggle() {
		this->on = !this->on;
	}
};

class SmartLight: public Device {
	size_t light;
public:
	SmartLight() {
		this->light = 0;
	}
	void off() {
		this->light = 0;
	}
	void on(size_t light = 50) {
		this->light = light % 101;
	}
	void increase(size_t d = 1) {
		this->light += d;
		this->light %= 101;
	}
	void decrease(size_t d = 1) {
		if (this->light <= d)
			this->light = 0;
		else
			this->light -= d;
	}
};

class FireAlarmSystem: public Device {
	bool alarm;
public:
	FireAlarmSystem() {
		this->alarm = false;
	}
	void on() {
		this->alarm = true;
	}
	void off() {
		this->alarm = false;
	}
};

class SecurityAlarmSystem: public Device {
	bool alarm;
public:
	SecurityAlarmSystem() {
		this->alarm = false;
	}
	void on() {
		this->alarm = true;
	}
	void off() {
		this->alarm = false;
	}
};

class CoffeeMachine: public Device {
	size_t regime;
	enum regimes {
		OFF, LATTE, CAPPUCINO, ESPRESSO, RISTRETTO
	};
public:
	CoffeeMachine() {
		this->regime = OFF;
	}

	void setRegime(size_t regime) {
		this->regime = regime % 5;
	}

	void off() {
		this->regime = OFF;
	}

	void makeCoffee() {
		switch (this->regime) {
		case OFF:
			cout << "Machine is off!" << endl;
			return;
		case LATTE:
			cout << "Making latte..." << endl << "\t Done!" << endl;
			return;
		case CAPPUCINO:
			cout << "Making cappucino..." << endl << "\t Done!" << endl;
			return;
		case ESPRESSO:
			cout << "Making espresso..." << endl << "\t Done!" << endl;
			return;
		case RISTRETTO:
			cout << "Making ristretto..." << endl << "\t Done!" << endl;
			return;
		}
	}
};

class MusicCenter: public Device {
	string song;
	bool on;
public:
	MusicCenter() {
		this->on = false;
		this->song = "";
	}
	void setSong(string song) {
		this->song = song;
	}
	void turnOn() {
		this->on = true;
	}
	void turnOff() {
		this->on = false;
	}
	void play() {
		if (!this->on) {
			cout << "Music center is off!" << endl;
			return;
		} else if (this->song == "") {
			cout << "No song selected!" << endl;
			return;
		} else {
			cout << "Playing " << this->song << endl;
			return;
		}
	}
};

class AbstractCommand {
public:
	virtual void execute()=0;
	virtual string name()=0;
	virtual ~AbstractCommand() {
	}
};

class AbstractObserver {
public:
	virtual void update(vector<AbstractCommand*> soft)=0;
	virtual ~AbstractObserver() {
	}
};

class SocketCommand;
class LightCommand;
class FireAlarmCommand;
class SecurityAlarmCommand;
class CoffeeMachineCommand;
class MusicCenterCommand;

class AbstractSubject {
public:
	virtual void attach(AbstractObserver*)=0;
	virtual void detach(AbstractObserver*)=0;
	virtual void notify()=0;
	virtual void addCommand(SocketCommand*)=0;
	virtual void addCommand(LightCommand*)=0;
	virtual void addCommand(FireAlarmCommand*)=0;
	virtual void addCommand(SecurityAlarmCommand*)=0;
	virtual void addCommand(CoffeeMachineCommand*)=0;
	virtual void addCommand(MusicCenterCommand*)=0;
	virtual void addCommand(AbstractCommand*)=0;
	virtual ~AbstractSubject() {
	}
};

class User: public AbstractObserver {
	AbstractSubject *subject;
	int id;
	vector<AbstractCommand*> soft;
public:
	User(AbstractSubject *subject) {
		this->id = rand();
		this->subject = subject;
		cout << "User " << this->id << " joins YandexPlus" << endl;
		this->subject->attach(this);
	}

	int getId() {
		return this->id;
	}

	void update(vector<AbstractCommand*> soft) {
		cout << "User " << this->id << " receives software updates" << endl;
		this->soft.clear();
		for (AbstractCommand *command : soft) {
			cout << "Adding command: " << command->name() << "..." << endl;
			this->soft.push_back(command);
		}
	}

	void listSoft() {
		cout << "User " << this->id << " has the following commands available:"
				<< endl;
		int i = 0;
		for (AbstractCommand *command : this->soft) {
			cout << i << ": " << command->name() << endl;
			i++;
		}
	}

	void run(size_t i){
		if(i>this->soft.size()){
			cout<<"Command #"<<i<<" does not exist!"<<endl;
			return;
		}
		this->soft[i]->execute();
	}

	~User() {
		cout << "User " << this->id << " leaves YandexPlus";
		this->subject->detach(this);
	}
};

class SocketCommand: public AbstractCommand {
protected:
	SmartSocket *smart_socket;
public:
	SocketCommand(SmartSocket *smart_socket) {
		this->smart_socket = smart_socket;
	}
	void set(SmartSocket *smart_socket) {
		this->smart_socket = smart_socket;
	}
	virtual string name()=0;
	virtual void execute()=0;
};

class SocketOn: public SocketCommand {
public:
	SocketOn(SmartSocket *smart_socket = nullptr) :
			SocketCommand(smart_socket) {
	}
	string name() {
		return "Turn Smart Socket on";
	}
	void execute() {
		this->smart_socket->turnOn();
	}
};

class SocketOff: public SocketCommand {
public:
	SocketOff(SmartSocket *smart_socket = nullptr) :
			SocketCommand(smart_socket) {
	}
	string name() {
		return "Turn Smart Socket off";
	}
	void execute() {
		this->smart_socket->turnOff();
	}
};

class LightCommand: public AbstractCommand {
protected:
	SmartLight *smart_light;
public:
	LightCommand(SmartLight *smart_light) {
		this->smart_light = smart_light;
	}
	void set(SmartLight *smart_light) {
		this->smart_light = smart_light;
	}
	virtual string name()=0;
	virtual void execute()=0;
};

class LightOn: public LightCommand {
public:
	LightOn(SmartLight *smart_light = nullptr) :
			LightCommand(smart_light) {
	}
	string name() {
		return "Turn Smart Light on";
	}
	void execute() {
		this->smart_light->on();
	}
};

class LightOff: public LightCommand {
public:
	LightOff(SmartLight *smart_light = nullptr) :
			LightCommand(smart_light) {
	}
	string name() {
		return "Turn Smart Light off";
	}
	void execute() {
		this->smart_light->off();
	}
};

class LightIncrease: public LightCommand {
	int by;
public:
	LightIncrease(int by, SmartLight *smart_light = nullptr) :
			LightCommand(smart_light) {
		this->by = by;
	}
	string name() {
		return "Increase light by " + to_string(this->by);
	}
	void execute() {
		this->smart_light->increase(by);
	}
};

class LightDecrease: public LightCommand {
	int by;
public:
	LightDecrease(int by, SmartLight *smart_light = nullptr) :
			LightCommand(smart_light) {
		this->by = by;
	}
	string name() {
		return "Decrease light by " + to_string(this->by);
	}
	void execute() {
		this->smart_light->decrease(by);
	}
};

class FireAlarmCommand: public AbstractCommand {
protected:
	FireAlarmSystem *alarm;
public:
	FireAlarmCommand(FireAlarmSystem *alarm) {
		this->alarm = alarm;
	}
	void set(FireAlarmSystem *alarm) {
		this->alarm = alarm;
	}
	virtual string name()=0;
	virtual void execute()=0;
};

class FireAlarmOn: public FireAlarmCommand {
public:
	FireAlarmOn(FireAlarmSystem *alarm = nullptr) :
			FireAlarmCommand(alarm) {
	}
	string name() {
		return "Set fire alarm on";
	}
	void execute() {
		this->alarm->on();
	}
};

class FireAlarmOff: public FireAlarmCommand {
public:
	FireAlarmOff(FireAlarmSystem *alarm = nullptr) :
			FireAlarmCommand(alarm) {
	}
	string name() {
		return "Set fire alarm off";
	}
	void execute() {
		this->alarm->off();
	}
};

class SecurityAlarmCommand: public AbstractCommand {
protected:
	SecurityAlarmSystem *alarm;
public:
	SecurityAlarmCommand(SecurityAlarmSystem *alarm) {
		this->alarm = alarm;
	}
	void set(SecurityAlarmSystem *alarm) {
		this->alarm = alarm;
	}
	virtual string name()=0;
	virtual void execute()=0;
};

class SecurityAlarmOn: public SecurityAlarmCommand {
public:
	SecurityAlarmOn(SecurityAlarmSystem *alarm = nullptr) :
			SecurityAlarmCommand(alarm) {
	}
	string name() {
		return "Set security alarm on";
	}
	void execute() {
		this->alarm->on();
	}
};

class SecurityAlarmOff: public SecurityAlarmCommand {
public:
	SecurityAlarmOff(SecurityAlarmSystem *alarm = nullptr) :
			SecurityAlarmCommand(alarm) {
	}
	string name() {
		return "Set security alarm off";
	}
	void execute() {
		this->alarm->off();
	}
};

class CoffeeMachineCommand: public AbstractCommand {
protected:
	CoffeeMachine *coffee_machine;
	enum regimes {
		OFF, LATTE, CAPPUCINO, ESPRESSO, RISTRETTO
	};
public:
	CoffeeMachineCommand(CoffeeMachine *coffee_machine) {
		this->coffee_machine = coffee_machine;
	}
	void set(CoffeeMachine *coffee_machine) {
		this->coffee_machine = coffee_machine;
	}
	virtual void execute()=0;
	virtual string name()=0;
	virtual ~CoffeeMachineCommand() {
	}
};

class CoffeeMachineOff: public CoffeeMachineCommand {
public:
	CoffeeMachineOff(CoffeeMachine *coffee_machine = nullptr) :
			CoffeeMachineCommand(coffee_machine) {
	}
	string name() {
		return "Turn the coffee machine off";
	}
	void execute() {
		this->coffee_machine->setRegime(OFF);
	}
};

class CoffeeMachineMakeLatte: public CoffeeMachineCommand {
public:
	CoffeeMachineMakeLatte(CoffeeMachine *coffee_machine = nullptr) :
			CoffeeMachineCommand(coffee_machine) {
	}
	string name() {
		return "Make Latte in the coffee machine";
	}
	void execute() {
		this->coffee_machine->setRegime(LATTE);
		this->coffee_machine->makeCoffee();
	}
};

class CoffeeMachineMakeCappucino: public CoffeeMachineCommand {
public:
	CoffeeMachineMakeCappucino(CoffeeMachine *coffee_machine = nullptr) :
			CoffeeMachineCommand(coffee_machine) {
	}
	string name() {
		return "Make Cappucino in the coffee machine";
	}
	void execute() {
		this->coffee_machine->setRegime(CAPPUCINO);
		this->coffee_machine->makeCoffee();
	}
};

class CoffeeMachineMakeEspresso: public CoffeeMachineCommand {
public:
	CoffeeMachineMakeEspresso(CoffeeMachine *coffee_machine = nullptr) :
			CoffeeMachineCommand(coffee_machine) {
	}
	string name() {
		return "Make Espresso in the coffee machine";
	}
	void execute() {
		this->coffee_machine->setRegime(ESPRESSO);
		this->coffee_machine->makeCoffee();
	}
};

class CoffeeMachineMakeRistretto: public CoffeeMachineCommand {
public:
	CoffeeMachineMakeRistretto(CoffeeMachine *coffee_machine = nullptr) :
			CoffeeMachineCommand(coffee_machine) {
	}
	string name() {
		return "Make Ristretto in the coffee machine";
	}
	void execute() {
		this->coffee_machine->setRegime(RISTRETTO);
		this->coffee_machine->makeCoffee();
	}
};

class MusicCenterCommand: public AbstractCommand {
protected:
	MusicCenter *music_center;
public:
	MusicCenterCommand(MusicCenter *music_center) {
		this->music_center = music_center;
	}
	void set(MusicCenter *music_center) {
		this->music_center = music_center;
	}
	virtual string name()=0;
	virtual void execute()=0;
};

class MusicCenterOn: public MusicCenterCommand {
public:
	MusicCenterOn(MusicCenter *music_center = nullptr) :
			MusicCenterCommand(music_center) {
	}
	string name() {
		return "Turn music center on";
	}
	void execute() {
		this->music_center->turnOn();
	}
};

class MusicCenterOff: public MusicCenterCommand {
public:
	MusicCenterOff(MusicCenter *music_center = nullptr) :
			MusicCenterCommand(music_center) {
	}
	string name() {
		return "Turn music center off";
	}
	void execute() {
		this->music_center->turnOff();
	}
};

class MusicCenterPlaySong: public MusicCenterCommand {
public:
	string song_name;
	MusicCenterPlaySong(string song_name, MusicCenter *music_center = nullptr) :
			MusicCenterCommand(music_center) {
		this->song_name = song_name;
	}
	string name() {
		return "Play \"" + this->song_name + "\" with the Music center";
	}
	void execute() {
		this->music_center->setSong(song_name);
		this->music_center->play();
	}
};

class YandexPlus: public AbstractSubject {
	list<AbstractObserver*> users;
	vector<Device*> devices;
	vector<AbstractCommand*> soft;
	enum devicenumbers {
		SOCKET, LIGHT, FIRE, SECURITY, COFFEE, MUSIC
	};
public:
	YandexPlus() {
		this->devices.push_back(new SmartSocket);
		this->devices.push_back(new SmartLight);
		this->devices.push_back(new FireAlarmSystem);
		this->devices.push_back(new SecurityAlarmSystem);
		this->devices.push_back(new CoffeeMachine);
		this->devices.push_back(new MusicCenter);
	}

	void attach(AbstractObserver *observer) {
		this->users.push_back(observer);
	}

	void detach(AbstractObserver *observer) {
		users.remove(observer);
	}

	void addCommand(SocketCommand *command) {
		cout << "YandexPlus receives command:" << command->name() << endl;
		command->set((SmartSocket*) this->devices[SOCKET]);
		this->soft.push_back(command);
	}

	void addCommand(LightCommand *command) {
		cout << "YandexPlus receives command:" << command->name() << endl;
		command->set((SmartLight*) this->devices[LIGHT]);
		this->soft.push_back(command);
	}

	void addCommand(FireAlarmCommand *command) {
		cout << "YandexPlus receives command:" << command->name() << endl;
		command->set((FireAlarmSystem*) this->devices[FIRE]);
		this->soft.push_back(command);
	}

	void addCommand(SecurityAlarmCommand *command) {
		cout << "YandexPlus receives command:" << command->name() << endl;
		command->set((SecurityAlarmSystem*) this->devices[SECURITY]);
		this->soft.push_back(command);
	}

	void addCommand(CoffeeMachineCommand *command) {
		cout << "YandexPlus receives command:" << command->name() << endl;
		command->set((CoffeeMachine*) this->devices[COFFEE]);
		this->soft.push_back(command);
	}

	void addCommand(MusicCenterCommand *command) {
		cout << "YandexPlus receives command:" << command->name() << endl;
		command->set((MusicCenter*) this->devices[MUSIC]);
		this->soft.push_back(command);
	}

	void addCommand(AbstractCommand *command) {
		cout << "Error! This method should never get called" << endl;
	}
	void notify() {
		for (auto user : this->users) {
			user->update(this->soft);
		}
	}
};

int main() {
	AbstractSubject *y = new YandexPlus;

	y->addCommand(new SocketOn);
	y->addCommand(new SocketOff);
	y->addCommand(new LightOn);
	y->addCommand(new LightOff);
	y->addCommand(new LightIncrease(50));
	y->addCommand(new LightDecrease(50));
	y->addCommand(new FireAlarmOn);
	y->addCommand(new FireAlarmOff);
	y->addCommand(new SecurityAlarmOn);
	y->addCommand(new SecurityAlarmOff);
	y->addCommand(new CoffeeMachineMakeLatte);
	y->addCommand(new CoffeeMachineMakeCappucino);
	y->addCommand(new CoffeeMachineMakeEspresso);
	y->addCommand(new CoffeeMachineMakeRistretto);
	y->addCommand(new CoffeeMachineOff);
	y->addCommand(new MusicCenterOn);
	y->addCommand(new MusicCenterOff);
	y->addCommand(
			new MusicCenterPlaySong("Pink Floyd - Shine on you crazy diamond"));
	y->addCommand(new MusicCenterPlaySong("Jethro Tull - Roots to Branches"));
	y->addCommand(
			new MusicCenterPlaySong("Mumford and Sons - Little Lion Man"));

	User *user = new User(y);
	y->notify();

	user->listSoft();
	cout<<"----"<<endl;
	user->run(17);
	user->run(15);
	user->run(17);

	return 0;
}
