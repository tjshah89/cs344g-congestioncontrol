#include <iostream>

#include "controller.hh"
#include "timestamp.hh"

using namespace std;

/* Default constructor */
Controller::Controller( const bool debug )
  : debug_( debug )
{}

/* Default: fixed window size of 100 outstanding datagrams */
unsigned int the_window_size = 20;

/* Get current window size, in datagrams */
unsigned int Controller::window_size( void )
{

  if ( debug_ ) {
    cerr << "At time " << timestamp_ms()
	 << " window size is " << the_window_size << endl;
  }

  return the_window_size;
}

/* A datagram was sent */
void Controller::datagram_was_sent( const uint64_t sequence_number,
				    /* of the sent datagram */
				    const uint64_t send_timestamp )
                                    /* in milliseconds */
{
  /* Default: take no action */

  if ( debug_ ) {
    cerr << "At time " << send_timestamp
	 << " sent datagram " << sequence_number << endl;
  }
}

/* An ack was received */
void Controller::ack_received( const uint64_t sequence_number_acked,
			       /* what sequence number was acknowledged */
			       const uint64_t send_timestamp_acked,
			       /* when the acknowledged datagram was sent (sender's clock) */
			       const uint64_t recv_timestamp_acked,
			       /* when the acknowledged datagram was received (receiver's clock)*/
			       const uint64_t timestamp_ack_received )
                               /* when the ack was received (by sender) */
{
    uint64_t upload_time = recv_timestamp_acked - send_timestamp_acked;
    if ( sequence_number_acked % the_window_size == 0 ) {
	if ( upload_time > 40  && upload_time < 80000) {
	    if (the_window_size > 7)
		the_window_size--;
	    cerr << "P Acked with upload time: " << upload_time << ", shrinking window size to "<<the_window_size<<"\n";
	}
	if ( upload_time > 0 && upload_time < 20 ) {
	    the_window_size++;
	    cerr << "P Acked with upload time: " << upload_time << ", growing t window size to "<<the_window_size<<"\n";
	}
    }
    else {
	if ( upload_time > 100  && upload_time < 80000) {
	    if (the_window_size > 7)
		the_window_size--;
	    cerr << "P Acked with upload time: " << upload_time << ", shrinking window size to "<<the_window_size<<"\n";
	}
    }

  if ( debug_ ) {
    cerr << "At time " << timestamp_ack_received
	 << " received ack for datagram " << sequence_number_acked
	 << " (send @ time " << send_timestamp_acked
	 << ", received @ time " << recv_timestamp_acked << " by receiver's clock)"
	 << endl;
  }
}

/* How long to wait (in milliseconds) if there are no acks
   before sending one more datagram */
unsigned int Controller::timeout_ms( void )
{
  return 1000; /* timeout of one second */
}
