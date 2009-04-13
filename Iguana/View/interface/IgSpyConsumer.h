#ifndef VIEW_IG_SPY_CONSUMER_H
# define VIEW_IG_SPY_CONSUMER_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "Iguana/Framework/interface/IgNet.h"
# include <deque>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class IgDataStorage;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

static sig_atomic_t s_stop = 0;

static void
onInterrupt(int /* sig */)
{
    s_stop++;
}

class IgSpyConsumer : public IgNet
{
public:
    IgSpyConsumer(bool verbose, const std::string &host, int port)
	: IgNet("iguana-consumer")
	{
	    logme() << "INFO: listening for data from " << host << ':' << port << '\n';
	    debug(verbose);
	    delay(100);
	    listenToSource(host, port);
	}

    static void
    report(void *arg, uint32_t reason, Object &o)
	{
	    IgSpyConsumer *self = static_cast<IgSpyConsumer *>(arg);
	    if (reason == VIS_FLAG_RECEIVED)
	    {
		if (o.flags & VIS_FLAG_NEW)
		{
		    o.flags &= ~VIS_FLAG_NEW;
		    std::ostream &x = self->logme();
		    x << "UPDATED: " << o.name << ", version " << o.version << ", "
		      << o.rawdata.size() << " bytes data, flags 0x"
		      << std::hex << o.flags << std::dec;
		    if (o.flags & VIS_FLAG_SCALAR)
		    {
			std::vector<char> data;
			data.reserve(o.rawdata.size() + 1);
			data.insert(data.end(), o.rawdata.begin(), o.rawdata.end());
			data.push_back(0);

			x << "; text: '" << &data[0] << "'\n";
		    }
		    else
			x << '\n';
		}
	    }
	    else if (reason == VIS_FLAG_DEAD)
		self->logme()
		    << "DELETED: " << o.name << ", version " << o.version
		    << ", flags 0x" << std::hex << o.flags << std::dec << '\n';
	    else
		self->logme()
		    << "ERROR: unrecognised object update code " << reason << '\n';
	}
  
    virtual bool
    shouldStop(void)
	{
	    receive(&report, this);
	    return s_stop > 0;
	}
private:
    std::vector<char> 	data_;
    IgDataStorage      *storage_;    
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

#endif // VIEW_IG_SPY_CONSUMER_H
