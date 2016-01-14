#ifndef _TIMER_
#define _TIMER_

#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>

using namespace std;

typedef unsigned long long tulong;

#ifdef __MACH__
#include <sys/time.h>
#warning "clock_gettime is not implemented on OSX"

#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 0

int clock_gettime(int /* clk_id */, struct timespec* t)
{
  struct timeval now;
  int rv = gettimeofday(&now, NULL);
  if (rv) return rv;
  t->tv_sec  = now.tv_sec;
  t->tv_nsec = now.tv_usec * 1000;
  return 0;
};

#endif//__MACH__

//Posix or Mac OS
#if defined(_POSIX_VERSION) || defined(__MACH__)

/** suffix string constants **/
const static char* suffix[] =
{
  "s",
  "ds",
  "cs",
  "ms",
  "us",
  "ns"
};

/** suffix string enum **/
enum eSuffix
{
  eSec = 0,
  eDsec,
  eCsec,
  eMsec,
  eUsec,
  eNsec
};

/**
 * A realtime precision timer for POSIX complient systems
 * Uses -lrt for gcc compiling
 */
class Timer
{
  /** The timespecs for this timer instances **/
  timespec startTime, endTime, curTime;

  /** The offset time for start stop and resume **/
  tulong offset;

  /** Indicates whether or not the timer is stopped or running **/
  bool stopped, started;

  /** This timers suffix enum **/
  eSuffix sfx;

  /** The string value suffix **/
  string ssfx;

  /**
   * Set the time for a timespec in ns
   */
  inline tulong getTime(timespec & ts)
  {
    return !clock_gettime(CLOCK_REALTIME, &ts) ? getTimeNSecs(ts) : 0;
  };

  /**
   * Retrieves the time unit suffix
   */
  inline const char* getSuffix(eSuffix s = eNsec)
  {
    return suffix[(int)s];
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
    sfx = eNsec;
    ssfx = getSuffix();
    offset = 0; stopped = true; started = false;
    getTime(curTime);
  };

  /**
   * Default construct and start if user specified
   * @param: bool, startTimer
   * @param: eSuffix, pSuffix
   */
  Timer(bool startTimer, eSuffix pSuffix = eNsec)
  {
    sfx = pSuffix;
    ssfx = getSuffix();
    offset = 0; stopped = true; started = false;
    getTime(curTime);
    if(startTimer) start();
  };

  /**
   * Output operator overload
   */
  friend ostream& operator << (ostream & o, Timer & t)
  {
    o << "Rendering Timer...\n"
      << "Start Time: " << t.getTimeNSecs(t.startTime) << t.ssfx << "\n"
      << "End Time: "   << t.getTimeNSecs(t.endTime)   << t.ssfx << "\n"
      << "Cur Time: "   << t.getTimeNSecs(t.curTime)   << t.ssfx << "\n"
      << "Offset: "     << t.getOffsetNSecs()          << t.ssfx << "\n"
      << "Elapsed: "    << t.getElapsedNSecs()         << t.ssfx << "\n"
      << "Duration: "   << t.getTimeNSecs(t.endTime) -
                           t.getTimeNSecs(t.startTime) -
                           t.getOffsetNSecs()          << t.ssfx << " : "
                        << t.getTimeSecs(t.endTime) -
                           t.getTimeSecs(t.startTime) -
                           t.getOffsetSecs()           << "s\n";

    return o;
  };

  /**
   * Subtraction operater overload computes time difference
   */
  tulong operator - (Timer & t)
  {
    tulong e1 =   getElapsedNSecs(),
           e2 = t.getElapsedNSecs();

    return e1 > e2 ? e1 - e2 : e2 - e1;
  }

  /**
   * Get curr time NS public asccessor
   */
  inline tulong getTimeNSecs()
  {
    return getTimeNSecs(curTime);
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
  inline string getTimeStr() const
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
  inline char* getCTime() const
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
    if(!stopped)
      stop();
    tulong ret = getTimeNSecs(endTime) - getTimeNSecs(startTime) - getOffsetNSecs();
    resume();

    return ret;
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
  inline void reset() { offset = 0; stopped = 1; started = 0; getTime(startTime); };

  /**
   * Indicates the timer is running
   */
  inline const bool& running() const { return started; };

  /**
   * Destructor ;)
   */
  ~Timer()
  {
    if(!stopped)
    {
      stop();
      cout << *this;
    }
  }
};

#else//defined(_POSIX_VERSION) || defined(__MACH__)

#warning "POSIX CLOCK_REALTIME Unavailable, using system clock()"

/**
 * The less percise time operates using system time
 */
class Timer
{
  clock_t startTime, endTime, curTime;

  tulong offset;

  bool stopped, started;

  public:

  Timer()                 { offset = 0; stopped = 1; started = 0; startTime = clock(); };
  inline void   start()   { curTime = startTime = clock(); stopped = 0; started = 1; };
  inline void   stop()    { curTime = endTime = clock(); stopped = 1; };
  inline void   reset()   { offset = 0; stopped = 1; started = 0; };
  inline tulong resume()  { offset = endTime - clock(); stopped = 0; return offset; };
  inline double elapsed() { return (clock() - startTime) / (double) CLOCKS_PER_SEC; };
  inline tulong ticks ()  { return (endTime - startTime - offset); };
  inline double duration(){ return ticks() / CLOCKS_PER_SEC; };
  ~Timer()                { if(!stopped) { stop(); } };
};

#endif//_POSIX_VERSION
#endif//_TIMER_
