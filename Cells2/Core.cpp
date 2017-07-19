#include "Core.h"

vector<Core*> Core::cores;
vector<Chunk*> Core::tasks;
mutex Core::taskMutex;
condition_variable Core::cv;

Core::Core()
{
	finished = false;
	stop = false;
	waiting = true;
	t = new thread(&Core::run, this);
}


Core::~Core()
{
	if (finished) {
		t->join();
	}
	else
	{
		t->detach();
	}
	delete t;
}

void Core::setup(int amount)
{
	for(int i=0;i<amount;i++)
	{
		cores.push_back(new Core());
	}
}

void Core::runChunk(Chunk* chunk)
{
	unique_lock<mutex> lck(taskMutex);
	tasks.push_back(chunk);
	cv.notify_one();
}

void Core::end()
{
	for(int i=0;i<cores.size();i++)
	{
		cores.at(i)->stop = true;
		delete cores.at(i);
	}
}

bool Core::allFinished()
{
	taskMutex.lock();
	if (tasks.size()>0)
	{
		taskMutex.unlock();
		return false;
	}
	taskMutex.unlock();
	for (int i = 0; i < cores.size(); i++)
	{
		if(!cores.at(i)->waiting)
		{
			return false;
		}
	}
	return true;
}

Chunk* Core::getNextChunk()
{
	unique_lock<mutex> lck(taskMutex);
	while (tasks.empty()) 
	{
		cv.wait(lck);
	}
	Chunk* extracted = tasks.at(0);
	tasks.erase(tasks.begin());
	return extracted;
}

void Core::run()
{
	while(!stop)
	{
		Chunk* chunk = getNextChunk();
		waiting = false;
		chunk->run();
		waiting = true;
	}
	finished = true;
}
