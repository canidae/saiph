#include "Embed.h"

#include "Debug.h"
#include "Globals.h"
#include "World.h"

#include <algorithm>

#include <dlfcn.h>
#include <pthread.h>

#include <stdlib.h>
#include <errno.h>
#include <string.h>

using namespace std;

typedef void (*nethack_set_hooks)(int(*)(), void(*)(const char*,int), void(*)(int));
typedef int  (*nethack_main)(int, const char**);

/* constructors/destructor */
Embed::Embed() {
	if (used) {
		Debug::error() << "Embed cannot be used more than once per process." << endl;
		World::destroy();
		exit(1);
	}
	used = 1;

	/* load nethack */
	void *handle = dlopen("./libnethack.so", RTLD_NOW);
	if (!handle) {
		Debug::error() << "Loading nethack failed: " << dlerror() << endl;
		World::destroy();
		exit(1);
	}

	nethack_set_hooks nsh = (nethack_set_hooks) dlsym(handle, "nethack_set_hooks_1");
	nethack_main nm = (nethack_main) dlsym(handle, "nethack_main_1");
	if (!nsh || !nm) {
		Debug::error() << "Loading nethack symbols failed: " << dlerror() << endl;
		World::destroy();
		exit(1);
	}

	std::vector<char> path(256);
	while (getcwd(&path[1], path.size() - 10) == NULL) { /* @ + /nethackrc\0 */
		if (errno == ERANGE) {
			path.resize(path.size() * 2);
		} else {
			Debug::error() << "getcwd failed: " << strerror(errno) << endl;
			World::destroy();
			exit(1);
		}
	}
	path[0] = '@';
	strcat(&path[0], "/nethackrc");
	setenv("NETHACKOPTIONS", &path[0], 1);

	(*nsh)(hook_input, hook_output, hook_exit);

	pthread_mutex_init(&mutex, 0);
	pthread_cond_init(&ready, 0);

	if (pthread_create(&threadid, 0, startNetHack, (void*) nm) != 0) {
		Debug::error() << "Thread creation failed" << endl;
		World::destroy();
		exit(1);
	}

}

Embed::~Embed() {
	transmit("Sy"); // save & quit
	char buf[1];
	retrieve(buf, 1);
	pthread_cancel(threadid);
	pthread_join(threadid, 0);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&ready);
}

/* methods */
void * Embed::startNetHack(void *arg) {
	const char* argv[] = {
		"nethack",
		NULL
	};
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
	(* (nethack_main) arg)(1, argv);
	return 0; // not reached?
}

int Embed::hook_input() {
	pthread_mutex_lock(&mutex);
	while (inputq.size() == 0) {
		iwaiting = 1;
		pthread_cond_signal(&ready);
		pthread_cond_wait(&ready, &mutex);
		iwaiting = 0;
	}
	int v = inputq.front();
	if (v >= 0) inputq.pop_front();
	pthread_mutex_unlock(&mutex);
	return v;
}

/* note: no locking is needed here, because, between this and the read in doRetrieve,
   a mutex unlock (after signal) here and lock in doRetrieve (in wait) must intervene. */
void Embed::hook_output(const char *s, int l) {
	outputq.insert(outputq.end(), s, s+l);
}

void Embed::hook_exit(int) {
	pthread_mutex_lock(&mutex);
	exited = 1;
	pthread_cond_signal(&ready);
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

int Embed::doRetrieve(char* buffer, int count) {
	pthread_mutex_lock(&mutex);
	while (!iwaiting && !exited)
		pthread_cond_wait(&ready, &mutex);

	/* retrieve data */
	ssize_t amount = std::min((ssize_t)count - 1, (ssize_t)outputq.size());
	std::copy(outputq.begin(), outputq.begin() + amount, buffer);
	outputq.empty();

	buffer[amount] = '\0';
	pthread_mutex_unlock(&mutex);
	return (int) amount;
}

int Embed::transmit(const string& data) {
	/* send data */
	pthread_mutex_lock(&mutex);
	inputq.insert(inputq.end(), data.begin(), data.end());
	pthread_cond_signal(&ready);
	pthread_mutex_unlock(&mutex);

	return data.size();
}

void Embed::start() {
	/* no need for some special code here */
}

void Embed::stop() {
}

/* global variables */
std::deque<char> Embed::inputq;
std::vector<char> Embed::outputq;
int Embed::iwaiting, Embed::exited, Embed::used;
pthread_t Embed::threadid;
pthread_cond_t Embed::ready;
pthread_mutex_t Embed::mutex;
