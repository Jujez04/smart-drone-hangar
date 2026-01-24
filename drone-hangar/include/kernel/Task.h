#ifndef __TASK__
#define __TASK__

enum TaskType {
    PERIODIC,
    APERIODIC
};

class Task {
public:
    Task() : active(false), completed(false) {}
    Task(TaskType t) : type(t), active(false), completed(false) {}
    virtual ~Task() {}

    virtual void tick() = 0; // template method

    // Getters and Setters
    bool isActive() const {
        return active;
    }

    bool isCompleted() const {
        return completed;
    }

    bool isPeriodic() const {
        return type == PERIODIC;
    }

    virtual void setActive(bool active) {
        this->active = active;
        if (active) completed = false;
    }

    TaskType getType() const { return type; }

protected:
    bool active;
    TaskType type = PERIODIC;
    bool completed;
};

#endif