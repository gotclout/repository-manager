#ifndef _TIMER_
#define _TIMER_

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>

using namespace std;

typedef unsigned long long tulong;

#ifdef _POSIX_VERSION

/**
 * A realtime pecision timer for POSIX complient systems
 * Use -lrt for gcc compiling
 */
class Timer
{
  /** The timespecs for this timer instances **/
  timespec startTime, endTime, curTime;

  /** The offset time for start stop and resume **/
  tulong offset;

  /** Indicates whether or not the timer is stopped or running **/
  bool stopped, started;

  /**
   * Set the time for a timespec in ns
   */
  inline tulong getTime(timespec & ts)
  {
    return !clock_gettime(CLOCK_REALTIME, &ts) ? getTimeNSecs(ts) : 0;
  };

  /**
   * get time ns
   */
  inline tulong getTimeNSecs(timespec & ts)
  {
    return (ts.tv_sec * 1e9) + ts.tv_nsec;
  };

  /**
   * get time micro sec
   */
  inline double getTimeUSecs(timespec & ts) { return getTimeNSecs(ts) * 0.0010; };

  /**
   * get time milisec
   */
  inline double getTimeMSecs(timespec & ts) { return getTimeUSecs(ts) * 0.0010; };

  /**
   * get time centisecs
   */
  inline double getTimeCSecs(timespec & ts) { return getTimeMSecs(ts) * 0.1000; };

  /**
   * get time decisecs
   */
  inline double getTimeDSecs(timespec & ts) { return getTimeCSecs(ts) * 0.1000; };

  /**
   * Retrieves the value of the timespec sec
   */
  inline double getTimeSecs(timespec & ts)  { return getTimeDSecs(ts) * 0.1000; };

  /**
   * get time offset (while paused until resume) nsec
   */
  inline tulong getOffsetNSecs() { return offset; };

  /**
   * get time offset (while paused until resume) USecs
   */
  inline double getOffsetUSecs() { return getOffsetNSecs() * 0.0010; };

  /**
   * get time offset (while paused until resume) milisecs
   */
  inline double getOffsetMSecs() { return getOffsetUSecs() * 0.0010; };

  /**
   * get time offset (while paused until resume) centisecs
   */
  inline double getOffsetCSecs() { return getOffsetMSecs() * 0.1000; };

  /**
   * get time offset (while paused until resume) decisec
   */
  inline double getOffsetDSecs() { return getOffsetCSecs() * 0.1000; };

  /**
   * get time offset (while paused until resume) sec
   */
  inline double getOffsetSecs()  { return getOffsetDSecs() * 0.1000; };

  public:

  /**
   * Default Constructor
   */
  Timer()
  {
    offset = 0; stopped = true; started = false;
    getTime(curTime);
  };

  /**
   * Ouput operator overload
   */
  friend ostream& operator << (ostream & o, Timer & t)
  {
    o << "Rendering Timer - "
      << "Start Time: " << t.getTimeNSecs(t.startTime) << " ns "
      << "End Time: "   << t.getTimeNSecs(t.endTime)   << " ns "
      << "Cur Time: "   << t.getTimeNSecs(t.curTime)   << " ns "
      << "Offset: "     << t.getOffsetNSecs()          << " ns "
      << "Elapsed: "    << t.getElapsedNSecs()         << " ns "
      << "Duration: "   << t.getTimeNSecs(t.endTime) -
                           t.getTimeNSecs(t.startTime) -
                           t.getOffsetNSecs()          << " ns : "
                        << t.getTimeSecs(t.endTime) -
                           t.getTimeSecs(t.startTime) -
                           t.getOffsetSecs()           << " s\n";

    return o;
  };

  /**
   * Starts the timer
   */
  tulong start()
  {
    tulong st = 0;

    if(stopped)
    {
      stopped = false;
      st = getTime(startTime);
      curTime = startTime;
      if(!started) started = true;
    }
    else
    {
      cerr << "ERROR: The timer has already been started. Use Timer::stop to "
           << "stop the timer or Timer::reset to reset it.\n";
    }

    return st;
  };

  /**
   * Stops the timer
   */
  tulong stop()
  {
    tulong d = 0;

    if(!stopped)
    {
      stopped = true;
      d = getTime(endTime);
      curTime = endTime;
    }
    else
    {
      cerr << "ERROR: Timer has not been started before stop."
           << " Use Timer::start() to start the timer.\n";
    }

    return d;
  };

  /**
   * Resumes a stopped timer
   */
  tulong resume()
  {
    if(stopped)
    {
      offset += (getTime(curTime) - getTimeNSecs(endTime));
      stopped = false;
    }
    else
    {
      cerr << "ERROR: Timer has not been stopped before resume."
           << " Use Timer::stop() to stop the timer.\n";
    }

    return offset;
  };

  /**
   * Compute the amount of time elapsed since the timer was last stopped
   * or checked
   */
  inline tulong getElapsedNSecs()
  {
    tulong d = 0;

    if(started)
    {
      timespec now;
      d = getTime(now) - getTimeNSecs(curTime);
      curTime = now;
    }
    else
    {
      cerr << "ERROR: Timer has not been started, no time has elapsed."
           << " Use Timer::start() to start the timer.\n";
    }

    return d;
  };

  /**
   * Elpased Time usec
   */
  inline double getElapsedUSecs() { return getElapsedNSecs() * 0.0010; };

  /**
   * Elpased Time msec
   */
  inline double getElapsedMSecs() { return getElapsedUSecs() * 0.0010; };

  /**
   * Elpased Time csec
   */
  inline double getElapsedCSecs() { return getElapsedMSecs() * 0.1000; };

  /**
   * Elpased Time dsec
   */
  inline double getElapsedDSecs() { return getElapsedCSecs() * 0.1000; };

  /**
   * Elpased Time sec
   */
  inline double getElapsedSecs()  { return getElapsedDSecs() * 0.1000; };

  /**
   *  Converts the current time to HH:MM:SS formatted string
   */
  inline string getTimeStr()
  {
    stringstream ss;
    time_t cur = time(0);
    struct tm* now = gmtime(&cur);

    if(now->tm_hour < 10) ss << "0";
    ss << now->tm_hour << ":";
    if(now->tm_min < 10) ss << "0";
    ss << now->tm_min << ":";
    if(now->tm_sec < 10) ss << "0";
    ss << now->tm_sec;

    return ss.str();
  };

  /**
   * Converts current time to Www Mmm dd hh:mm:ss yyyy cstring
   */
  inline char* getCTime()
  {
    time_t cur;
    time(&cur);
    return ctime(&cur);
  };

  /**
   * Compute the duration of the interval from start to stop
   */
  inline tulong getDurationNSecs()
  {
    if(!stopped) stop();

    return getTimeNSecs(endTime) - getTimeNSecs(startTime) - getOffsetNSecs();
  };

  /**
   * Compute duration usec
   */
  inline double getDurationUSecs() { return getDurationNSecs() * 0.0010; };

  /**
   * Compute duration msec
   */
  inline double getDurationMSecs() { return getDurationUSecs() * 0.0010; };

  /**
   * Compute duration csec
   */
  inline double getDurationCSecs() { return getDurationMSecs() * 0.1000; };

  /**
   * Compute duration dsec
   */
  inline double getDurationDSecs() { return getDurationCSecs() * 0.1000; };

  /**
   * Compute duration sec
   */
  inline double getDurationSecs() { return getDurationDSecs() * 0.1000; };

  /**
   * Reset the timer
   */
  inline void reset() { offset = 0; stopped = true; };

  /**
   * Destructor ;)
   */
  ~Timer()
  {
    if(!stopped)
    {
      stop();
      cerr << *this;
    }
  }
};

#else

#warning "POSIX CLOCK_REALTIME Unavailable, using system clock()"

/**
 * The less percise time operates on system time
 */
class Timer
{
  clock_t startTime, endTime, curTime;

  tulong offset;

  bool stopped;

  public:

  Timer()          { offset = 0; stopped = true; };
  void   start()   { startTime = clock(); curTime = startTime; stopped = true; };
  void   stop()    { endTime = clock(); curTime = endTime; stopped = true; };
  void   reset()   { offset = 0; stopped = true; };
  tulong resume()  { offset = endTime - clock(); stopped = 0; return offset; };
  double elapsed() { return (clock() - startTime) / (double) CLOCKS_PER_SEC; };
  tulong ticks ()  { return (endTime - startTime - offset); };
  double duration(){ return ticks() / (double) CLOCKS_PER_SEC; };
};

#endif//_POSIX_VERSION
#endif//_TIMER_
