/***
 *** See the file "mba/disclaimers-and-notices-L2.txt" for
 *** information on usage and redistribution of this file,
 *** and for a DISCLAIMER OF ALL WARRANTIES.
 ***/

/* $Id: checkpoint.h,v 1.9 2004/01/28 23:31:33 lbrown Exp $ */
#ifndef checkpoint_HEADER
#define checkpoint_HEADER

#include <livingstone/L2_iostream.h>
#include <livingstone/l2conf.h>
#include <mba_utils/MBA_string.h>

class Livingstone;
class Tracker;

/**
 * Defines an API for providing checkpoints to those who want them.
 * There is no limit to how many checkpoints are outstanding.
 * The user is responsible for keeping track of checkpoints ; the 
 * Livingstone engine does not.
 */
class L2_checkpoint : public Pooled {
    public:
        /**
         * Create a new checkpoint, storing the current state of Livingstone
         * such that it can later be re-created.
         */
        L2_checkpoint(Livingstone*);

        /**
         * Identical to above, but for internal use only.  If accidental
         * contact with eyes occurs, flush thoroughly.
         */
        L2_checkpoint(Tracker*);

        /**
          * Destroy the checkpoint and all storage associated with it.
          */
        virtual ~L2_checkpoint();

        /**
         * Restore Livingstone to the state stored when this object was
         * created.  The current state of Livingstone is overwritten.
         * This checkpoint can be restored as often as wanted.
         * Returns false if the current state of the engine is 
         * inapproriate in a way we can't easily fix.
         */
        virtual bool restore();

#ifdef ENABLE_CHECKPOINT_SERIALIZE
	/**
	 * Write the checkpoint object to a file
	 * \param xmlCheckpointFilePath pathname of the file for writing the
	 * serialized checkpoint
	 */
	virtual bool serialize(const MBA_string& xmlCheckpointFilePath);

	/**
	 * Write the checkpoint object to a stream
	 * \param os the output stream for writing the serialized checkpoint
	 */
	virtual bool serialize(_STD_ ostream& os);

	/**
	 * Create a checkpoint from a file
	 * \param xmlCheckpointFilePath pathname of the file for reading the
	 * serialized checkpoint
	 */	
	L2_checkpoint(Tracker* tracker,
		      const MBA_string& xmlCheckpointFilePath);

	/**
	 * Create a checkpoint from an input stream
	 * \param is the input stream for reading the serialized checkpoint
	 */
	virtual bool isValid() { return valid; }
#endif

    private:
        /**
          * The data item is opaque and does all the real work.
          */
        class data;
        data *data_;
	class CheckpointHandler;
        bool valid;
};

#endif
