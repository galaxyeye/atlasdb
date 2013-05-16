/*
 * task_flow.h
 *
 *  Created on: May 1, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_SYSTEM_TASK_H_
#define ATLASDB_SYSTEM_TASK_H_

#include <queue>
#include <functional>

#include <boost/uuid/uuid.hpp>
#include <boostpp/threadpool/pool.hpp>

#include <atlas/singleton.h>

namespace atlasdb {
  namespace system {

    using boost::uuids::uuid;

    typedef std::function<void()> task_type;
    typedef atlas::singleton<boostpp::threadpool::fifo_pool> the_worker_pool;

    class task {
    public:

      task(const uuid& id, task_type&& t) : _id(id), _task(t) {}

      const uuid& id() const;
      bool paused() const;

      void operator()() { _task(); }
      operator bool() const { return _task; }

    private:

      task_type _task;
      const uuid& _id;
    };

    class task_runner {
    public:

      void run() {
        the_worker_pool::ref().schedule(std::bind(&task_runner::do_run, this));
      }

      void enqueue(const task& task) { _tasks.push(task); }

      void enqueue(task&& task) { _tasks.push(task); }

      void suspend(task&& task) { _tasks.push(task); }

    protected:

      const uuid& do_run() {
        task task = _tasks.front();
        _tasks.pop();

        if (!task) return 0;

        uuid task_id = task.id();

        if (task.paused()) {
          suspend(std::move(task));
        }

        return task_id;
      }

    private:

      std::queue<task> _tasks;
    };

  } // system
} // atlasdb

#endif /* ATLASDB_SYSTEM_TASK_H_ */
