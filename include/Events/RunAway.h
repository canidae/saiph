#ifndef EVENT_RUN_AWAY_H
#define EVENT_RUN_AWAY_H

#include "Coordinate.h"
#include "Events/Event.h"
#include "Data/Monster.h"

namespace event {

    class RunAway : public Event {
    public:
        static const int ID;

        RunAway() : Event("RunAway"), _runningFrom() {
        }

        virtual ~RunAway() {
        }

        virtual int id() {
            return ID;
        }

        virtual Monster runningFrom() {
            return runningFrom();
        }

    private:
        Monster _runningFrom;
