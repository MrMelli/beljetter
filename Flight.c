#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct flight_list_node {
	int flightno;          /* !< The flight number */
	char dep[20];          /* !< Departure airport code */
	char des[20];          /* !< Destination airport code */
	char datestr[20];      /* !< Date departure*/
	char timestr[20];      /* !< Time departure */
	int nfs;               /* !< Number of First class rows */
	int *fs;	             /* !< An array with flags of if a seat is taken or not. 1==occupied, 0==not occupied */
	int nbs;               /* !< Number of Business class rows */
	int *bs;               /* !< An array with flags of if a seat is taken or not. 1==occupied, 0==not occupied */
	int nes;               /* !< Number of Economy class rows */
	int *es;               /* !< An array with flags of if a seat is taken or not. 1==occupied, 0==not occupied */
	struct flight_list_node *next; /* !< A pointer to the next flight information */
} FlightListNode;

typedef struct booking_list_node {
	int booking;          /* !< The booking number */
	char datestr[15];     /* !< The departure date */
	char timestr[15];     /* !< The departure time */
	char dep[10];         /* !< The departure airport */
	char des[10];         /* !< The destination airport */
	char class[20];       /* !< The seat class */
	char fname[25];       /* !< Firstname */
	char lname[25];       /* !< Lastname */
	struct booking_list_node *next; /* !< a pointer to the next booking */
} BookingListNode;

FlightListNode *read_flights(const char *filename)
{
	FlightListNode fln;              /* A variable to store a row read from the file */
	FlightListNode  *head=NULL;      /* A pointer to the first node in the linked list */
	FILE *fp = fopen(filename,"r");  /* Open the file where flight data is */
	while( fscanf(fp,"%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n",&fln.flightno,fln.dep,fln.des,fln.datestr,fln.timestr,&fln.nfs,&fln.nbs,&fln.nes) == 8 ) {
	FlightListNode *nn = malloc(sizeof(FlightListNode)); /* Allocate space to save the read information */
	memcpy(nn,&fln,sizeof(FlightListNode));              /* Copy the read data into the allocated memory */
	nn->fs = malloc(fln.nfs*sizeof(int)*7);              /* Allocate a vector to keep track of seat booking in first class */
	memset(nn->fs,0,fln.nfs*7);                          /* Initiate the seats to free, e.g 0 */
	nn->bs = malloc(fln.nbs*sizeof(int)*7);              /* Allocate a vector to keep track of seat booking in business class */
	memset(nn->bs,0,fln.nbs*7);                          /* Initiate the seats to free, e.g 0 */
	nn->es = malloc(fln.nes*sizeof(int)*7);              /* Allocate a vector to keep track of seat booking in economy class */
	memset(nn->es,0,fln.nes*7);                          /* Initiate the seats to free, e.g 0 */
	nn->next =  head;                                    /* Set the next pointer to head .... */
	head = nn;                                           /* then set the head to the new node. */
	}
	return(head);                                        /* Return the pointer to the list with flights read from the file */
}
int
allocate_seat (FlightListNode * flight, BookingListNode * booking, int *row,
  int *seat);
  void
print_ticket (BookingListNode * blnp, FlightListNode * flnp, int seat,
  int row);

/**
* @brief Read bookings from a file, return a linked list with the booking information.
*/
BookingListNode *read_bookings(const char *filename) 
{
	BookingListNode bln;               /* A variable to store a row read from the file */
	BookingListNode *head=NULL;       /* A pointer to the first node in the linked list */
	FILE *fp = fopen(filename,"r");  /* Open the file where flight data is */
	//bookingnumber,date,time,departure,destination,seatclass,firstname,surname
	while( fscanf(fp,"%d,%[^,],%[^,],%[^,],%[^,],%d,%d,%d\n",&bln.booking,bln.datestr,bln.timestr,bln.dep,bln.des,bln.class,bln.fname,bln.lname) == 8 ) {
	BookingListNode *nn = malloc(sizeof(FlightListNode)); /* Allocate space to save the read information */
	memcpy(nn,&bln,sizeof(FlightListNode));              /* Copy the read data into the allocated memory */
	
}
return(head); 
}
/**
* @brief Read flights from a file, return a linked list with the flights information.
*/

int create_tickets(BookingListNode *bookings, FlightListNode *flights)
{
	int num_tickets = 0;
	fprintf(stdout,"Writing tickets: ");


	  /* Loop through the booking list */

	  for(BookingListNode *blnp = bookings; blnp != NULL; blnp=blnp->next) {

    /* find the flight for this booking, see hint 6 */
    /* Loop through the flights list ... */
		for(FlightListNode *flnp = flights; flnp != NULL; flnp=flnp->next) {

		  /* Check if this flight have the same departure, destination, 
       date and time as the booking have */
			if( !strcmp(blnp->dep,flnp->dep) &&  
			    !strcmp(blnp->des,flnp->des) &&  
          !strcmp(blnp->datestr,flnp->datestr) &&  
          !strcmp(blnp->timestr,flnp->timestr) ) {
        /* allocate seat and mark it booked */
        /* see hint 7 */
        int row=0, seat=0;

				/* have booking and flight, now find a seat, mark it booked 
           and print the ticket */

				if(allocate_seat(flnp,blnp,&row,&seat)){ /* hint 8 */
					fprintf(stdout,"[ticket-%d.txt]",blnp->booking);
					print_ticket(blnp,flnp,seat,row); /* hint 9 */
					num_tickets++;
				}
			}
		}


	}
	fprintf(stdout,"Created  %d tickets\n\n",num_tickets);
	return(num_tickets);
}


void
print_ticket (BookingListNode * blnp, FlightListNode * flnp, int seat,
  int row)
{
  char filename[255];
  sprintf (filename, "ticket-%d.txt", blnp->booking);
  FILE *fp = fopen (filename, "w");
  if (fp)
  {
    fprintf (fp, "BOOKING:%d\n", blnp->booking);
    fprintf (fp, "FLIGHT:%d DEPARTURE:%s DESTINATION: %s %s %s\n",
    flnp->flightno, flnp->dep, flnp->des, flnp->datestr,
    flnp->timestr);
    fprintf (fp, "PASSENGER %s %s\n", blnp->fname, blnp->lname);
    fprintf (fp, "CLASS: %s\n", blnp->class);
    fprintf (fp, "ROW %d SEAT %d\n\n", row, seat);
    fclose (fp);
  }
    
}


int
allocate_seat (FlightListNode * flight, BookingListNode * booking, int *row,
    int *seat)
{
  int sn = 0;
  int rn = 0;
  /* is the booking in first class ? */
  if (strcmp ("first", booking->class) == 0)
    {
      /* Yes, now find a free seat in first class */
      for (int p = 0; p < flight->nfs * 7; p++)
    {
      if (flight->fs[p] == 0)
        {            /* seat is free */
          flight->fs[p] = 1;    /* set it booked */
          sn = p + 1;    /* calculate seat number */
          rn = (int) p / 7 + 1;    /* calculate row number */
          break;        /* no need to look for more */
        }
    }
    }
  /* is the booking in business class ? */
  if (strcmp ("business", booking->class) == 0)
    {
      /* Yes, now find a free seat in business class */
      for (int p = 0; p < flight->nbs * 7; p++)
    {
      if (flight->bs[p] == 0)
        {            /* seat is free */
          flight->bs[p] = 1;    /* set it booked */
          sn = p + flight->nfs * 7 + 1;    /* calculate seat number */
          rn = flight->nfs + (int) p / 7 + 1;    /* calculate row number */
          break;        /* no need to look for more */
        }
    }
    }
  /* is the booking in economy class ? */
  if (strcmp ("economy", booking->class) == 0)
    {
      /* Yes, now find a free seat in economy class */
      for (int p = 0; p < flight->nes * 7; p++)
    {
      if (flight->es[p] == 0)
        {            /* seat is free */
          flight->es[p] = 1;    /* set it booked */
          sn = p + flight->nfs * 7 + flight->nbs * 7 + 1;    /* calculate seat number */
          rn = flight->nfs + flight->nbs + (int) p / 7 + 1;    /* calculate row number */
          break;        /* no need to look for more */
        }
    }
    }
  if (rn == 0 || sn == 0)
    {
      fprintf (stdout,
        "did not find class \"%s\" for booking on this plane\n",
        booking->class);
      return (0);
    }
  *row = rn;
  *seat = sn;
 
  return (1);
}
