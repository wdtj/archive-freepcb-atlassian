// PcbItem.h:  Proposal for a new class hierarchy encompassing all the items that
//   appear on a PCB.  By having a single base class, cpcb_item, and using pointers to objects 
//   belonging to derived classes, it should be possible to supplant the old system of id's.
// Also included are new template classes, carray<T> and citer<T>;  we can rely mostly on them
//   instead of CArray<T>/CArray<T*> and the various CIter_XXX classes.  carray and citer have a number of
//   safety features, including: (1) if a member of a carray gets deleted, references to it 
//   in the carray will be removed automatically;  (2) if a member of a carray gets detached or
//   deleted in the midst of a loop, citer can handle the situation gracefully;  (3) if a carray as
//   a whole gets deleted in the midst of a loop, its members are detached and citer handles that too;  (4) removal of items 
//   from a carray, and detection of their presence in the carray, are quick.

#pragma once
#include <afxcoll.h>
#include <afxtempl.h>
#include "DisplayList.h"
#include "PartList.h"
#include "PolyLine.h"
#include "UndoList.h"
#include "LinkList.h"
#include "Cuid.h"

class cpcb_item;
class carray_link;
template <class T> class carray;  // NB "T" is expected to be a cpcb_item class
template <class T> class citer;	  // Ditto

class cvertex2;
class ctee;
class cseg2;
class cconnect2;
class cpin2;
class cpart2;
class ccorner;			// TODO: rename ccorner
class cside;			// TODO: rename cside
class ccontour;
class cpolyline;			// TODO: rename cpolyline
class carea2;
class csmcutout;
class cboard;
class coutline;
class cnet2;
class ctext;				// TODO: rename ctext
class creftext;
class cvaluetext;
class ccentroid;
class cadhesive;
class cdrc;

class undo_con;
class undo_seg;
class undo_vtx;

class cnetlist;
class CFreePcbDoc;

enum typebits {
	// Bit flags to represent the types of the various types of PCB items.  The main use for these will be in implementing selection masks.
	bitVia =			0x1,
	bitPinVtx =			0x2,
	bitTraceVtx =		0x4,
	bitTeeVtx =			0x8,
	bitTee =			0x10,
	bitSeg =			0x20,
	bitConnect =		0x40,
	bitPin =			0x80,
	bitPart =			0x100,
	bitAreaCorner =		0x200,
	bitAreaSide =		0x400,
	bitSmCorner =		0x800,
	bitSmSide =			0x1000,
	bitBoardCorner =	0x2000,
	bitBoardSide =		0x4000,
	bitOutlineCorner =	0x8000,			// Within the footprint editor only...
	bitOutlineSide =	0x10000,		// Within the footprint editor only...
	bitContour =		0x20000,
	bitArea =			0x40000,
	bitSmCutout =		0x80000,
	bitBoard =			0x100000,
	bitOutline =		0x200000,		// Fp editor only...
	bitNet =			0x400000,
	bitText =			0x800000,
	bitRefText =		0x1000000,
	bitValueText =		0x2000000,
	bitCentroid =		0x4000000,		// Fp editor only...
	bitAdhesive =		0x8000000,		// Fp editor only...
	bitDRC =			0x10000000,
	bitOther =			0x80000000,
	bitsNetItem = bitVia | bitPinVtx | bitTraceVtx | bitTeeVtx | bitTee | bitSeg | bitConnect |
				  bitAreaCorner | bitAreaSide | bitArea,
	bitsPartItem = bitPin | bitPart
};

class cpcb_item 
{
	// THE BASE CLASS FOR EVERY ITEM ON A PCB.  
	// Best way I could figure out to give the various carray types access to cpcb_item's innards was to list 'em as friends 1 by 1.
	// An ugly PITA, but I suppose necessary:
	friend class carray<cpcb_item>;
	friend class carray<cvertex2>;
	friend class carray<ctee>;
	friend class carray<cseg2>;
	friend class carray<cconnect2>;
	friend class carray<cpin2>;
	friend class carray<cpart2>;
	friend class carray<ccorner>;
	friend class carray<cside>;
	friend class carray<ccontour>;
	friend class carray<cpolyline>;
	friend class carray<carea2>;
	friend class carray<csmcutout>;
	friend class carray<cboard>;
	friend class carray<coutline>;
	friend class carray<cnet2>;
	friend class carray<ctext>;
	friend class carray<creftext>;
	friend class carray<cvaluetext>;
	friend class carray<ccentroid>;
	friend class carray<cadhesive>;
	friend class carray<cdrc>;

	carray_link *carray_list;	// List of carray's into which this item has been added
	int m_uid;
	static int next_uid;
public:							// ?
	CFreePcbDoc *doc;
	dl_element *dl_el;			// Do enough of the derived classes want it to merit its inclusion?
	dl_element *dl_sel;			// Ditto...
	int utility;

protected:
	cpcb_item(CFreePcbDoc *_doc);
	~cpcb_item();								// Had to define this in the .cpp.  When an item is destroyed, references to it are automatically
												// removed from any carrays to which it belongs.
public:
	int UID() { return m_uid; }

	// Virtual functions:
	virtual	void Draw() { }
	virtual void Undraw() { }
	virtual BOOL IsDrawn() { return false; }
	virtual void Highlight() { }
	// Type identification functions.  All return false by default, but are overridden in specified derived classes.
	virtual bool IsVertex() { return false; }
	virtual bool IsVia() { return false; }
	virtual bool IsSlaveVtx() { return false; }
	virtual bool IsPinVtx() { return false; }
	virtual bool IsEndVtx() { return false; }
	virtual bool IsTraceVtx() { return false; }
	virtual bool IsTee() { return false; }
	virtual bool IsSeg() { return false; }
	virtual bool IsConnect() { return false; }
	virtual bool IsPin() { return false; }
	virtual bool IsPart() { return false; }
	virtual bool IsCorner() { return false; }
	virtual bool IsSide() { return false; }
	virtual bool IsContour() { return false; }
	virtual bool IsPolyline() { return false; }
	virtual bool IsArea() { return false; }
	virtual bool IsSmCutout() { return false; }
	virtual bool IsBoard() { return false; }
	virtual bool IsOutline() { return false; }
	virtual bool IsNet() { return false; }
	virtual bool IsText() { return false; }			// Also false for creftext and cvaluetext
	virtual bool IsRefText() { return false; }
	virtual bool IsValueText() { return false; }
	virtual bool IsCentroid() { return false; }
	virtual bool IsAdhesive() { return false; }
	virtual bool IsDRC() { return false; }

	virtual int GetTypeBit() { return 0; }			// See "enum typebits" above for return values from the various derived classes
	bool IsPartItem() { return (GetTypeBit() & bitsNetItem) != 0; }
	bool IsNetItem() { return (GetTypeBit() & bitsPartItem) != 0; }

	// Type casting functions.  All return null by default, but are overridden to return type-cast pointers in specified derived classes
	virtual cvertex2 *ToVertex() { return NULL; }
	virtual ctee *ToTee() { return NULL; }
	virtual cseg2 *ToSeg() { return NULL; }
	virtual cconnect2 *ToConnect() { return NULL; }
	virtual cpin2 *ToPin() { return NULL; }
	virtual cpart2 *ToPart() { return NULL; }
	virtual ccorner *ToCorner() { return NULL; }
	virtual cside *ToSide() { return NULL; }
	virtual ccontour *ToContour() { return NULL; }
	virtual cpolyline *ToPolyline() { return NULL; }
	virtual carea2 *ToArea() { return NULL; }
	virtual csmcutout *ToSmCutout() { return NULL; }
	virtual cboard *ToBoard() { return NULL; }
	virtual coutline *ToOutline() { return NULL; }
	virtual cnet2 *ToNet() { return NULL; }
	virtual ctext *ToText() { return NULL; }
	virtual creftext *ToRefText() { return NULL; }
	virtual cvaluetext *ToValueText() { return NULL; }
	virtual ccentroid *ToCentroid() { return NULL; }
	virtual cadhesive *ToAdhesive() { return NULL; }
	virtual cdrc *ToDRC() { return NULL; }
};

class carray_link
{
	// Used in forging linked lists of carray<T> pointers (which are declared below as void pointers for simplicity).  When a cpcb_item is
	// added to a carray, one of these links is hooked to its carray_list.  In that way we know which carrays it belongs to (I don't foresee
	// many items belonging to more than 1 or 2 arrays in general).  We can also use the "off" member of this structure to determine what position
	// the item has in the indicated array.
	// Same dumb business with friends as in cpcb_item :(
	friend class cpcb_item;
	friend class carray<cpcb_item>;
	friend class carray<cvertex2>;
	friend class carray<ctee>;
	friend class carray<cseg2>;
	friend class carray<cconnect2>;
	friend class carray<cpin2>;
	friend class carray<cpart2>;
	friend class carray<ccorner>;
	friend class carray<cside>;
	friend class carray<ccontour>;
	friend class carray<cpolyline>;
	friend class carray<carea2>;
	friend class carray<csmcutout>;
	friend class carray<cboard>;
	friend class carray<coutline>;
	friend class carray<cnet2>;
	friend class carray<ctext>;
	friend class carray<creftext>;
	friend class carray<cvaluetext>;
	friend class carray<ccentroid>;
	friend class carray<cadhesive>;
	friend class carray<cdrc>;

	void *arr;									// Really a carray<T> pointer, for some T.
	int off;
	carray_link *next;
};


template <class T> class carray 
{
	// An array designed to hold items in classes derived from cpcb_item.  Note that entries in these arrays are pointers, not objects (so 
	// that carray<cpin2> is more like CArray<cpin2*> than CArray<cpin2>).  Each cpcb_item has an attached linked list of the carrays to which it
	// belongs, which allows various maintenance chores to be done automatically.  When an item is removed from a carray, the slot it occupied
	// is marked as free and linked to a list for later reuse.
	// For ease of reference and for a potential inline speed-boost, I put the code here in the .h
	friend class cpcb_item;
	friend class citer<T>;
	T **heap;					// Expandable array of data pointers.
	unsigned char *flags;		// Bitset, indicating which elements of "heap" contain valid entries.  A zero bit indicates a free slot.
								// Bits beyond position "maxOff" are undefined.
	int nHeap;					// Total number of slots in heap
	int nItems;					// Number of filled slots
	int free;					// The head of the free slot list (an index into "heap").  -1 => everything through maxOff is filled
	int maxOff;					// Maximum offset among the slots that are either filled, or hooked to the free list
	citer<T> *iters;			// Linked list of iterators that are actively looping through this.  Typically I don't expect more than 1 or 2...

public:
	carray(int nHeapInit = 16)
	{
		// Create the array, with the given number of initial entries in the heap.
		heap = (T**) HeapAlloc(GetProcessHeap(), 0, nHeapInit*sizeof(T**));
		flags = (unsigned char*) HeapAlloc(GetProcessHeap(), 0, nHeapInit/8 + 1);
		nHeap = nHeapInit;
		nItems = 0;
		maxOff = free = -1;
		iters = NULL;
	}

	~carray()
	{
		// Destructor must remove all references to this from its member items.  But it doesn't destroy the members.
		RemoveAll();
		HeapFree(GetProcessHeap(), 0, heap);
		HeapFree(GetProcessHeap(), 0, flags);
		// Blank out references to this from active iterators:
		for (citer<T> *iter = iters; iter; iter = iter->nextIter)
			iter->arr = NULL;
	}

	void Add(T* item)
	{
		// Add item to this carray.  But first check if the item already belongs to this, and return silently if so (?)
		for (carray_link *link = item->carray_list; link; link = link->next)
			if (link->arr==this) return;
		// See if heap needs to expand.  It grows by a factor of ~1.5 each time:
		if (nItems==nHeap)
		{
			int newNHeap = nHeap*3/2 + 1;
			T** newHeap = (T**) HeapAlloc(GetProcessHeap(), 0, newNHeap*sizeof(T**));
			unsigned char *newFlags = (unsigned char*) HeapAlloc(GetProcessHeap(), 0, (newNHeap/8) + 1);
			CopyMemory(newHeap, heap, nHeap*sizeof(T**));
			CopyMemory(newFlags, flags, nHeap/8 + 1);
			HeapFree(GetProcessHeap(), 0, heap);
			HeapFree(GetProcessHeap(), 0, flags);
			heap = newHeap;
			flags = newFlags;
			nHeap = newNHeap;
		}
		// Determine offset where item will be placed:
		int off; 
		if (free>=0)
			off = free,
			free = *(int*) heap[off];
		else
			off = ++maxOff;
		// Add item!  Then create a carray_link that connects the item with this carray.
		heap[off] = item;
		flags[off>>3] |= 1<<(off&7);						// Set flag for "off"
		carray_link *link = new carray_link();
		link->off = off;
		link->arr = this;
		link->next = item->carray_list;
		item->carray_list = link;
		nItems++;
	}

	void Add(carray<T> *arr) 
	{
		// Append the members of "arr" onto this.
		for (int i=0; i<=arr->maxOff; i++) 
			if (arr->flags[i>>3] & (1<<(i&7)))
				Add(arr->heap[i]);
	}

	bool Contains(T* item)
	{
		// Return true if "item" is in the array.
		for (carray_link *link = item->carray_list; link; link = link->next)
			if (link->arr == this) return true;
		return false;
	}

	void Remove(T* item)
	{
		// Remove "item" from this.  If item isn't in the list, do an ASSERT (?)
		carray_link *link, *prev = NULL;
		for (link = item->carray_list; link; prev = link, link = link->next)
			if (link->arr == this) break;
		ASSERT(link!=0);
		// Unhook and destroy "link" (after extracting its offset info)
		int off = link->off;
		if (!prev) item->carray_list = link->next;
		else       prev->next = link->next;
		delete link;
		// Remove item!
		ASSERT(heap[off]==item);			// For now...
		heap[off] = (T*) free;				// Add "off" to the free-offset list
		free = off;
		flags[off>>3] &= ~(1<<(off&7));		// Clear flag for "off"
		nItems--;
	}

	void RemoveAll()
	{
		// Remove all items from array (but don't destroy them).
		for (int i=0; i<=maxOff; i++) 
			if (flags[i>>3] & (1<<(i&7)))
				Remove(heap[i]);
		maxOff = free = -1;							// Ensures that if array fills up again, iteration through it will be efficient.
	}

	void DestroyAll()
	{
		// Destroy all items in array (and their destructors will also ensure that the array is empty at the end)
		for (int i=0; i<=maxOff; i++) 
			if (flags[i>>3] & (1<<(i&7)))
				delete heap[i];
		maxOff = free = -1;							// Ensures that if array fills up again, iteration through it will be efficient.
	}

	int GetNItems() { return nItems; }
	
	T *FindByUID(int uid)
	{
		for (int i=0; i<=maxOff; i++)
			if (flags[i>>3] & (1<<(i&7)))
				if (heap[i]->UID() == uid)
					return heap[i];
		return NULL;
	}

	bool ReadItems(T** out, int ct) 
	{
		// Read "ct" items from the carray into "out". Return false if ct is too large.  Maybe unneeded.
		if (ct>nItems) return false;
		int j = 0;
		for (int i=0; j<ct; i++)
			if (flags[i>>3] & (1<<(i&7)))
				out[j++] = heap[i];
		return true;
	}
};


template <class T> class citer
{
	// Use these classes to iterate through the members of a carray<T>.  That way we can handle it gracefully if an item is removed from the array 
	//  from within the loop, and even if the array as a whole is destroyed.
	// Sample usage:
	//		citer<cseg2> iseg (con->m_segs);
	//		for (cseg2 *seg = iseg.First(); seg; seg = iseg.Next())
	//			...
	friend class carray<T>;
	carray<T> *arr;
	int nextOff;								// Next offset at which Next() will seek a new entry.
	citer<T> *nextIter;							// Active iterators for a particular array are linked (singly) in a list.

public:
	citer(carray<T> *arr0)
	{
		arr = arr0;
		nextOff = 0;
		// Hook this to arr's list of iterators.
		nextIter = arr->iters;
		arr->iters = this;
	}

	~citer()
	{
		// Remove this iterator from its array's list of iterators.  Since the list is singly-linked, the process may be inefficient;  
		//   but I expect the list to be very short and the overall savings should outweight the penalty.
		if (!arr) return;								// Array has been destroyed!
		if (arr->iters==this)
			arr->iters = nextIter;
		else 
		{
			citer<T> *prev = NULL;
			for (citer<T> *iter = arr->iters; iter; iter = iter->nextIter)
				if (iter->nextIter==this) 
					{ prev = iter; break; }
			ASSERT(prev!=NULL);
			prev->nextIter = nextIter;
		}
	}

	T* Next() 
	{
		if (!arr) return NULL;							// Array has been destroyed!
		for ( ; nextOff <= arr->maxOff; nextOff++)
			if (arr->flags[nextOff>>3] & (1<<(nextOff&7)))
				return arr->heap[nextOff++];
		return NULL;
	}

	T* First()
		{ nextOff = 0; return Next(); }
};



/**********************************************************************************************/
/*  RELATED TO cseg2/cvertex2/cconnect2                                                          */
/**********************************************************************************************/

// cvertex2: describes a vertex between segments in a connection
class cvertex2: public cpcb_item
{
public:
	/* types of vertices.  CPT2:  can probably do without.
	enum VType { 
		V_ERR = 0,	// unknown
		V_PIN,		// pin at end of trace
		V_TRACE,	// vertex between trace segments
		V_END,		// end vertex of stub trace
		V_TEE,		// T-vertex (defined here)
		V_SLAVE		// T-vertex (defined in another trace)
	};	
	*/

	cconnect2 * m_con;			// parent connection
	cnet2 *m_net;				// CPT2.  Very likely useful.
	cseg2 *preSeg, *postSeg;		// Succeeding and following segs.  If either is null, this is an end seg.
	ctee *tee;					// Points to a tee structure at tee-junctions, null otherwise
	cpin2 *pin;					// If this vertex is at a pin, point to the said object
	int x, y;					// coords
	// CPT2: put into cpin2 (?) int pad_layer;				// layer of pad if this is first or last vertex, otherwise 0
	int force_via_flag;			// force a via even if no layer change
	int via_w, via_hole_w;		// via width and hole width (via_w==0 means no via)
	int utility2;				// used for various functions
	CArray<dl_element*> dl_els;	// CPT2 renamed;  contains one dl_element per layer (CHECK INTO THIS)
	dl_element * dl_hole;		// hole in via
	// CDisplayList * m_dlist;		// NULL if not drawable.  CPT2 use doc->m_dlist (?)

	cvertex2(cconnect2 *c, int _x=0, int _y=0);				// CPT2 Added args. Done in cpp
	cvertex2( cvertex2& src );
	~cvertex2()
	{
	}

	// cvertex2 &operator=( const cvertex2 &v );  // assignment	
	// cvertex2 &operator=( cvertex2 &v );		// assignment
	// void Initialize( cconnect2 * c );			// Move to constructor
	bool IsVertex() { return true; }
	bool IsVia() { return via_w>=0; }
	bool IsSlaveVtx() { return tee!=NULL; }
	bool IsPinVtx() { return pin!=NULL; }
	bool IsEndVtx() { return (!preSeg || !postSeg) && !pin && !tee; }
	bool IsTraceVtx() { return preSeg && postSeg; }
	cvertex2 *ToVertex() { return this; }
	int GetTypeBit()
	{ 
		if (via_w>=0) return bitVia;
		if (pin) return bitPinVtx;
		if (tee) return bitTeeVtx;							// NB tee-vertices will typically NOT be selectable, but tees will.
		return bitTraceVtx;
	}

	bool Remove();											// Done in cpp

	void Draw() { }											// TODO
	void Undraw() { }										// TODO
	void Highlight() { }									// TODO

	void GetTypeStatusStr( CString * str );
	void GetStatusStr( CString * str );
	bool IsViaNeeded();										// Done in cpp
	void ReconcileVia();									// Done in cpp
	bool IsConnectedToArea( carea2 * a );
	int GetConnectedAreas( carray<carea2> *a=NULL );		// CPT2. Arg change
};

class ctee: public cpcb_item 
{
	// CPT2 new.  Basically a carray<cvertex2>, indicating the vertices that together join up to form a single tee junction.
public:
	carray<cvertex2> vtxs;
	int via_w, via_hole_w;					// These will be the max of the values in the constituent vtxs.

	ctee(cnet2 *n);							// Done in cpp
	~ctee() 
	{
		// References to the tee from its constituent vertices must be erased.
		citer<cvertex2> iv (&vtxs);
		for (cvertex2 *v = iv.First(); v; v = iv.Next())
			v->tee = NULL;
		vtxs.RemoveAll();
	}
	bool IsTee() { return true; }
	ctee *ToTee() { return this; }
	int GetTypeBit() 
		{ return via_w? bitVia: bitTee; }
	void Remove();										// Done in cpp
	bool Adjust();										// Done in cpp
	void ReconcileVia();								// Done in cpp
	void Remove(cvertex2 *v);							// Done in cpp
};


// cseg2: describes a segment of a connection 
class cseg2: public cpcb_item
{
public:
	// static int m_array_step;
	int m_layer;
	int m_width;
	int m_curve;
	bool m_selected;
	// CDisplayList * m_dlist;	// CPT2 use doc->m_dlist (?)
	cnet2 * m_net;				// parent net
	cconnect2 * m_con;			// parent connection
	cvertex2 *preVtx, *postVtx;	// CPT2

	enum Curve { STRAIGHT = 0,
			CCW,
			CW
	};

	cseg2(cconnect2 *c, int _layer, int _width);							// CPT2 added args.  Replaces Initialize().  Done in cpp
	~cseg2() 
	{
		// Destructor is not responsible for unhooking seg from preVtx and postVtx.
		Undraw();
	}
	cseg2( cseg2& src );							// copy constructor
	// cseg2& operator=( cseg2& rhs );			// assignment
	// cseg2& operator=( const cseg2& rhs );		// assignment
	bool IsSeg() { return true; }
	cseg2 *ToSeg() { return this; }
	int GetTypeBit() { return bitSeg; }

	void SetWidth( int w, int via_w, int via_hole_w );			// Done in cpp
	void SetLayer( int _layer )
	{
		// CPT2.  TODO integrate this first draft with old CNetList::ChangeSegmentLayer
		bool bWasDrawn = IsDrawn();
		if (bWasDrawn) Undraw();
		m_layer = _layer;
		if (bWasDrawn) Draw();
	}

	void Draw() { }												// TODO
	void Undraw() { }											// TODO
	BOOL IsDrawn() { return false; }							// TODO

	void SetVisibility( int vis );
	int Index();												// CPT2:  maybe, maybe not...
	void GetStatusStr( CString * str, int width = 0 );			// CPT added width param
	void Divide( cvertex2 *v, cseg2 *s, int dir );				// Done in cpp
	int Route( int layer, int width );							// Done in cpp
	void Unroute(int dx, int dy, int end);						// Done in cpp
	bool UnrouteWithoutMerge(int dx, int dy, int end);			// Done in cpp
	bool RemoveMerge(int end);									// Done in cpp
	bool RemoveBreak();											// Done in cpp
};

// cconnect2: describes a sequence of segments, running between two end vertices of arbitrary type (pin, tee, isolated end-vertex...)
class cconnect2: public cpcb_item
{
	// friend class CNetList;
	// friend class cnet2;
public:
	enum Dir {
		ROUTE_FORWARD = 0,
		ROUTE_BACKWARD
	};

	cnet2 * m_net;				// parent net
	carray<cseg2> segs;			// array of segments. NB not necessarily in geometrical order
	carray<cvertex2> vtxs;		// array of vertices. NB ditto
	cvertex2 *head, *tail;		// CPT2 for geometrical traversing.	

	bool locked;				// true if locked (will not be optimized away)
	BOOL m_bDrawn;				// drawn into display list or not.  CPT2 TODO move to base class (would affect contructor too)?
	// CDisplayList * m_dlist;	// CPT2 use doc->m_dlist (?)

	// these params used only by DRC
	int min_x, max_x;			// bounding rect
	int min_y, max_y;
	BOOL vias_present;			// flag to indicate that vias are present. 
	int seg_layers;				// mask for all layers used by segments.  CPT2 MUST STUDY

// methods
public:
	// general
	cconnect2( cnet2 * _net );	// Done in cpp
	~cconnect2()
	{
		// Destructor does not delete constituent segs and vtxs.  If you want to get rid of all those, call DestroyAll() before deleting.
		Undraw();
		// If constituent segs and vtxs point back to this, change their ptrs to NULL
		citer<cseg2> is (&segs);
		for (cseg2 *s = is.First(); s; s = is.Next())
			if (s->m_con == this) s->m_con = NULL;
		citer<cvertex2> iv (&vtxs);
		for (cvertex2 *v = iv.First(); v; v = iv.Next())
			if (v->m_con == this) v->m_con = NULL;
	}

	bool IsConnect() { return true; }
	cconnect2 *ToConnect() { return this; }
	int GetTypeBit() { return bitConnect; }			// Rarely used since connects don't have selector elements.

	// void ClearArrays();							// CPT2 unused
	void GetStatusStr( CString * str );

	// get info about start/ending pins
	cpin2 * StartPin() { return head->pin; }
	cpin2 * EndPin() { return tail->pin; }
	BOOL TestHitOnEndPad( int x, int y, int layer, int dir );	// CPT2 TODO:  adapt from old CNetList::TestHitOnConnectionEndPad

	// get info about vertices
	int NumVtxs() { return vtxs.GetNItems(); }
	// cvertex2 * FirstVtx();
	// cvertex2 * LastVtx();
	// cvertex2 * VtxByUID( int uid, int * index=NULL );        // Use vtxs.FindByUID() instead
	cvertex2 *VtxByIndex( int iv );								// CPT2: maybe, maybe not...  changed from cvertex2& ret-val to cvertex2*
	int VtxIndexByPtr( cvertex2 * v );							// CPT2: maybe, maybe not...

	// get info about segments
	int NumSegs() { return segs.GetNItems(); }
	// void SetNumSegs( int n );
	cseg2 *FirstSeg() { return head->postSeg; }
	cseg2 *LastSeg() { return tail->preSeg; }
	// cseg2 *SegByUID( int uid, int * index=NULL );				// Use segs.FindByUID().
	cseg2 *SegByIndex( int is );									// CPT2: maybe, maybe not....
	int SegIndexByPtr( cseg2 * s );									// CPT2: ditto
	// int SegUIDByPtr( cseg2 * s );

	void SetWidth( int w, int via_w, int via_hole_w );				// Done in cpp

	// modify segments and vertices. 
	void Remove(bool bSetAreaConnections = true);					// Done in cpp
	// cvertex2& InsertVertexByIndex( int iv, const cvertex2& new_vtx );		// CPT2.  Currently called only with iv==0.  Not really compatible with
																				// the new system, hopefully dumpable
	// void InsertSegAndVtxByIndex( int is, int dir, const cseg2& new_seg, const cvertex2& new_vtx );	// CPT2.  Replace by:
	void AppendSegAndVertex( cseg2 *s, cvertex2 *v, cvertex2 *after) ;			// Done in cpp
	// void AppendSegAndVertex( const cseg2& new_seg, const cvertex2& new_vtx );  // CPT2.  Use AppendSegAndVertex( new_seg, new_vtx, tail)
	// void PrependVertex( const cvertex2& new_vtx );							// CPT2.  Probably able to live without.  
	void PrependVertexAndSeg( cvertex2 *v, cseg2 *s );					// Done in cpp
	void AppendSegment( int x, int y, int layer, int width );           // Done in cpp
	void ReverseDirection();											// Done in cpp
	void CombineWith( cconnect2 *c2, cvertex2 *v1, cvertex2 *v2) ;		// Done in cpp
	void MergeUnroutedSegments();										// Done in cpp
	void ChangePin( int end_flag, cpin2 *newPin );						// CPT2 TODO adapt from CNetList::ChangeConnectionPin()

	// drawing methods
	void Draw() { }														// TODO
	void Undraw() { }													// TODO
	void HighLight() { }												// CPT2 TODO adapt from CNetList::HighlightConnection()
};


/**********************************************************************************************/
/*  RELATED TO cpin2/cpart2                                                                     */
/**********************************************************************************************/

// cpin2: describes a pin in a net.  Folded the old part_pin into this.
class cpin2 : public cpcb_item
{
public:
	// CString ref_des;		// reference designator such as 'U1'.  CPT2 READ OFF of part->ref_des
	CString pin_name;		// pin name such as "1" or "A23"
	cpart2 * part;			// pointer to part containing the pin.
	int x, y;				// position on PCB
	int pad_layer;			// CPT2. layer of pad on this pin (was in cvertex2)
	cnet2 * net;			// pointer to net, or NULL if not assigned.
	drc_pin drc;			// drc info
	dl_element * dl_hole;	// pointer to graphic element for hole
	CArray<dl_element*> dl_els;	// array of pointers to graphic elements for pads
	CArray<dl_element*> dl_thermals;					// CPT2.  TODO figure out if this works.

	cpin2(cpart2 *_part, cnet2 *_net);					// CPT2. Added args. Done in cpp
	cpin2( cpin2& src );
	~cpin2();

	bool IsPin() { return true; }
	cpin2 *ToPin() { return this; }
	int GetTypeBit() { return bitPin; }

	void SetThermalVisible(int layer, bool bVisible) { }			// CPT2.  TODO figure this out.

	// void Initialize( cnet2 * net );  // Put in constructor
	void Disconnect(bool bSetAreas = true) ;						// Done in cpp
};


// class cpart2 represents a part
class cpart2: public cpcb_item
{
public:
	// The partlist will be a carray, so no links required:
	// cpart2 * prev;		// link backward
	// cpart2 * next;		// link forward
	CPartList * m_pl;	// parent partlist
	carray<cpin2> pins;				// array of all pins in part
	BOOL drawn;			// TRUE if part has been drawn to display list.  ?? TODO Put into base class?
	BOOL visible;		// 0 to hide part                                ?? TODO Put into base class?
	int x,y;			// position of part origin on board
	int side;			// 0=top, 1=bottom
	int angle;			// orientation, degrees CW
	BOOL glued;			// 1=glued in place
	BOOL m_ref_vis;		// TRUE = ref shown
	int m_ref_xi;		// ref text params (relative to part)
	int m_ref_yi;	
	int m_ref_angle; 
	int m_ref_size;
	int m_ref_w;
	int m_ref_layer;	// layer if part is on top	
	BOOL m_value_vis;	// TRUE = value shown
	int m_value_xi;		// value text params (relative to part)
	int m_value_yi; 
	int m_value_angle; 
	int m_value_size; 
	int m_value_w;
	int m_value_layer;	// layer if part is on top
	// In base: dl_element * dl_sel;		// pointer to display list element for selection rect
	CString ref_des;			// ref designator such as "U3"
	dl_element * dl_ref_sel;	// pointer to selection rect for ref text 
	CString value;				// "value" string
	dl_element * dl_value_sel;	// pointer to selection rect for value 
	CString package;			// package (from original imported netlist, may be "")
	CShape * shape;				// pointer to the footprint of the part, may be NULL
	CArray<stroke> ref_text_stroke;		// strokes for ref. text // TODO CArray or carray?
	CArray<stroke> value_stroke;		// strokes for ref. text
	CArray<stroke> m_outline_stroke;	// array of outline strokes

	// drc info
	BOOL hole_flag;	// TRUE if holes present
	int min_x;		// bounding rect of pads
	int max_x;
	int min_y;
	int max_y;
	int max_r;		// max. radius of pads
	int layers;		// bit mask for layers with pads
	// flag used for importing
	BOOL bPreserve;	// preserve connections to this part

	cpart2( CPartList * pl )			// CPT2 TODO.  Will probably add more args...
		: cpcb_item (pl->doc)
	{ 
		m_pl = pl;
		// zero out pointers
		dl_ref_sel = 0;
		shape = 0;
	}

	~cpart2();

	bool IsPart() { return true; }
	cpart2 *ToPart() { return this; }
	int GetTypeBit() { return bitPart; }

	cpin2 * PinByUID( int uid, int * index=NULL );
	int GetNumRefStrokes();
	int GetNumValueStrokes();
	int GetNumOutlineStrokes();
	void OptimizeConnections( BOOL bBelowPinCount, int pin_count, BOOL bVisibleNetsOnly=TRUE );	// CPT2 TODO:  derive from old CNetList function
};


/**********************************************************************************************/
/*  RELATED TO cpolyline/carea2/csmcutout                                                      */
/**********************************************************************************************/

// TODO: Rename ccorner, cside, cpolyline

class ccorner: public cpcb_item
{
public:
	int x, y;
	ccontour *contour;				// CPT2.
	// dl_element * dl_corner_sel;  // Use cpcb_item::dl_sel
	cside *preSide, *postSide;		// CPT2

	ccorner(ccontour *_contour, int _x, int _y);		// Done in cpp
	ccorner( ccorner& src );	// copy constructor
	~ccorner();

	bool IsCorner() { return true; }
	ccorner *ToCorner() { return this; }
	int GetTypeBit();				// Done in cpp
	void Highlight() { }			// CPT2 TODO. Derive from old CPolyLine::HighlightCorner().
};

class cside: public cpcb_item
{
public:
	int m_style;					// TODO change to style?
	ccontour *contour;				// CPT2
	// dl_element * dl_side;		// Use base dl_el
	// dl_element * dl_side_sel;	// Use base dl_sel
	ccorner *preCorner, *postCorner;

	cside(ccontour *_contour);
	cside( cside& src );
	~cside();
	//	cside& operator=( const cside& rhs );	// assignment
	//	cside& operator=( cside& rhs );	// assignment
	bool IsSide() { return true; }
	cside *ToSide() { return this; }
	int GetTypeBit();
	void Highlight() { }	// CPT2 TODO. Derive from cpolyline::HighlightSide()
	void SetVisible();		// CPT2
	bool IsOnCutout();		// Done in cpp
};

class ccontour: public cpcb_item
{
public:
	// All CPT2.
	carray<ccorner> corners;	// Array of corners.  NB entries not necessarily in geometrical order
	carray<cside> sides;		// Array of sides
	ccorner *head, *tail;	// For when we need to trace thru geometrically.
	cpolyline *poly;			// The parent polyline

	ccontour(cpolyline *_poly, bool bMain);		// Done in cpp
	~ccontour();
	bool IsContour() { return true; }
	ccontour *ToContour() { return this; }
	int GetTypeBit() { return bitContour; }

	void Highlight() 
	{
		citer<cside> is (&sides);
		for (cside *s = is.First(); s; s = is.Next())
			s->Highlight();
	}

	CRect GetCornerBounds();
	BOOL TestPointInside( int x, int y );
	void Close()
	{
		if (head==tail) return;
		cside *s = new cside(this);
		s->preCorner = tail;
		s->postCorner = head;
		tail->postSide = s;
		head->preSide = s;
		tail = head;
	}
	void Unclose();				// TODO.  Write something analogous...
};

class cpolyline: public cpcb_item
{
public:
	enum { STRAIGHT, ARC_CW, ARC_CCW };	// side styles
	enum { NO_HATCH, DIAGONAL_FULL, DIAGONAL_EDGE }; // hatch styles

	// CDisplayList * m_dlist;		// display list // CPT2 use doc->m_dlist (?)

	cpcb_item *parent;			// CPT2.  TODO Probably obsolete...
	ccontour *main;				// CPT2
	carray<ccontour> contours;	// CPT2.  Secondary contours
	// id m_parent_id;			// id of the parent of the polyline
	// void * m_ptr;			// pointer to parent object (or NULL)
	int m_layer;				// layer to draw on
	int m_w;					// line width
	int m_sel_box;				// corner selection box width/2
	int m_hatch;				// hatch style, see enum above
	int m_nhatch;				// number of hatch lines
	CArray <dl_element*>  dl_hatch;	// hatch lines.  Use CArray with dl-elements generally?
	gpc_polygon * m_gpc_poly;	// polygon in gpc format
	polygon * m_php_poly;
	BOOL bDrawn;

public:
	// constructors/destructor
	cpolyline(CFreePcbDoc *_doc)
		: cpcb_item (_doc)
		{ }
	~cpolyline()
		{ }
	bool IsPolyline() { return true; }
	cpolyline *ToPolyline() { return this; }

	// functions for creating and modifying polyline
	void MarkConstituents();								// CPT2. Done in cpp.
	void Clear();
	void Start( int layer, int w, int sel_box, int x, int y,
		int hatch, id * id, void * ptr, BOOL bDraw=TRUE );
	void AppendCorner( int x, int y, int style = STRAIGHT );
	void InsertCorner( ccorner *c, int x, int y );			// Arg change.  Insert prior to "corner"
	void DeleteCorner( ccorner *c );						// Arg change.
	BOOL MoveCorner( ccorner *c, int x, int y, BOOL bEnforceCircularArcs=FALSE );	// Put into ccorner?
	void Close( int style = STRAIGHT );
	void RemoveContour( ccontour *ctr );					// Arg change
	// drawing functions
	// void HighlightSide( int is );		// Change to cside::Highlight();
	// void HighlightCorner( int ic );		// sim.
	void StartDraggingToInsertCorner( CDC * pDC, ccorner *c, int x, int y, int crosshair = 1 ); // Arg change
	void StartDraggingToMoveCorner( CDC * pDC, ccorner *c, int x, int y, int crosshair = 1 );	// Arg change
	void CancelDraggingToInsertCorner( ccorner *c );											// Arg change
	void CancelDraggingToMoveCorner( ccorner *c );												// Arg change
	void Undraw() { }																			// CPT2 TODO
	void Draw( CDisplayList * dl = NULL );
	void Hatch();
	void MakeVisible( BOOL visible = TRUE );
	void MoveOrigin( int x_off, int y_off );
	// void SetSideVisible( int is, int visible );  // Use cside::SetVisible()

	// misc. functions
	CRect GetBounds();
	CRect GetCornerBounds();
	// CRect GetCornerBounds( int icont );			// Use ccontour::GetCornerBounds()
	void Copy( cpolyline * src );
	BOOL TestPointInside( int x, int y );
	// BOOL TestPointInsideContour( int icont, int x, int y ); // Use ccontour fxn
	int TestIntersection( cpolyline * poly );
	void AppendArc( int xi, int yi, int xf, int yf, int xc, int yc, int num );	// Should we put this in ccontour?

	// undo functions
	int SizeOfUndoRecord();
	void SetFromUndo( undo_poly * un_poly );
	void CreatePolyUndoRecord( undo_poly * un_poly );

	// access functions
	// id Id();
	// int UID();   in base class
	int NumCorners();				// Also in ccontour?
	int NumSides();					// Ditto
	bool IsClosed()
		{ return main->head==main->tail; }
	int NumContours();
	// int Contour( int ic );				// Use ccorner::contour
	// int ContourStart( int icont );		// Similarly...
	// int ContourEnd( int icont );
	// int ContourSize( int icont );
	// int CornerIndexByUID( int uid );		// Unlikely to be useful
	// int SideIndexByUID( int uid );		// Ditto
	// int X( int ic );						// Use ccorner::x
	// int Y( int ic );
	// int EndContour( int ic );			// Use ctr->postSide==NULL
	// int Utility(){ return utility; }						// Just use the base class data member?
	// int Utility( int ic ){ return corner[ic].utility; };
	int Layer() { return m_layer; }							// Put in the header...
	int W() { return m_w; }
	// int CornerUID( int ic ){ return corner[ic].m_uid; };
	// int SideUID( int is );
	// int SideStyle( int is );
	// void * Ptr(){ return m_ptr; };
	int SelBoxSize();
	int GetHatch() { return m_hatch; }					// CPT2. Hard to get excited about these...
	BOOL Drawn() { return bDrawn; };
	gpc_polygon * GetGpcPoly(){ return m_gpc_poly; }

	// void SetParentId( id * id );
	// void SetUID( int uid );					// Seems fishy...
	// void SetCornerUID( int ic, int uid );	// Put in ccorner or dump...
	// void SetSideUID( int is, int uid );
	// void SetClosed( BOOL bClosed );			// Use ccontour::Close() and ccontour::Unclose()
	// void SetX( int ic, int x );
	// void SetY( int ic, int y );
	void SetEndContour( int ic, BOOL end_contour );
	// void SetUtility( int u ){ utility = u; };								// Maybe just make cpcb_item::utility public...
	// void SetUtility( int ic, int utility ){ corner[ic].utility = utility; }  // Use corner->utility = ...
	void SetLayer( int layer );
	void SetW( int w );
	void SetSideStyle( int is, int style, BOOL bDraw=TRUE );
	void SetSelBoxSize( int sel_box );
	void SetHatch( int hatch )
		{ Undraw(); m_hatch = hatch; Draw(); };
	void SetDisplayList( CDisplayList * dl );
	void SetDrawn( BOOL drawn )
		{ bDrawn = drawn; }
	void Highlight()													// CPT2 Derives from old CNetList::HighlightAreaSides
	{
		main->Highlight();
		citer<ccontour> ic (&contours);
		for (ccontour *c = ic.First(); c; c = ic.Next())
			c->Highlight();
	}

	// GPC functions
	int MakeGpcPoly( int icontour=0, CArray<CArc> * arc_array=NULL );
	int FreeGpcPoly();
	int NormalizeWithGpc( CArray<cpolyline*> * pa=NULL, BOOL bRetainArcs=FALSE );
	int RestoreArcs( CArray<CArc> * arc_array, CArray<cpolyline*> * pa=NULL );
//	cpolyline * MakePolylineForPad( int type, int x, int y, int w, int l, int r, int angle );
//	void AddContourForPadClearance( int type, int x, int y, int w, 
//						int l, int r, int angle, int fill_clearance,
//						int hole_w, int hole_clearance, BOOL bThermal=FALSE, int spoke_w=0 );
	void ClipGpcPolygon( gpc_op op, cpolyline * poly );

	// PHP functions
	int MakePhpPoly();
	void FreePhpPoly();
	void ClipPhpPolygon( int php_op, cpolyline * poly );
};

// carea2: describes a copper area in a net.
class carea2 : public cpolyline
{
public:
	cnet2 * m_net;		// parent net
	carray<cpin2> pins;	// CPT2: replaces npins, pin, PinByIndex() below.  Set up by SetAreaConnections() below.
	// CArray<dl_element*> dl_thermal;		// graphics for thermals on pins.  CPT2 Use cpin2::dl_thermal (TODO check that this works)
	CArray<dl_element*> dl_via_thermal; // graphics for thermals on stubs
	int nvias;							// number of via connections to area
	carray<cconnect2> vcon;				// Changed from CArray<int>.  DON'T YET UNDERSTAND USAGE
	carray<cvertex2> vtx;				// Ditto
	int utility2;

	carea2(cnet2 *_net);					// Done in cpp
	// carea2( const carea2& source);		// dummy copy constructor
	~carea2();						
	// carea2 &operator=( carea2 &a );		// dummy assignment operator
	// carea2 &operator=( const carea2 &a );	// dummy assignment operator

	void Initialize( CDisplayList * dlist, cnet2 * net );		// TODO Rethink relationship to constructor??
	bool IsArea() { return true; }
	carea2 *ToArea() { return this; }
	int GetTypeBit() { return bitArea; }

	void SetConnections() { }									// CPT2 TODO Derive from old CNetList::SetAreaConnections(cnet2*, int)

	// The following supplanted by using pin, vcon, and vtx members above.
	// int NumVertices();
	// cvertex2 * VtxByIndex( int iv );
	// int NumPins();
	//	cpin2 * PinByIndex( int ip );
	//	int npins;			// number of thru-hole pins within area on same net
	//	CArray<int> pin;	// array of thru-hole pins.
	//	cpin2 * PinByIndex( int ip );
	// CArray<int> vcon;	// connections 
	// CArray<int> vtx;	// vertices
};


class csmcutout : public cpolyline
{
	// Represents solder-mask cutouts, which are always closed polylines
public:
	csmcutout(CFreePcbDoc *_doc);				// Done in cpp
	~csmcutout() { }
	bool IsSmCutout() { return true; }
	csmcutout *ToSmCutout() { return this; }
	int GetTypeBit() { return bitSmCutout; }
};


class cboard : public cpolyline
{
public:
	// Represents board outlines.
	cboard(CFreePcbDoc *_doc);					// Done in cpp
	~cboard() { }
	bool IsBoard() { return true; }
	cboard *ToBoard() { return this; }
	int GetTypeBit() { return bitBoard; }
};

class coutline : public cpolyline
{
public:
	// Represents outlines within footprints
	coutline(CFreePcbDoc *_doc);				// Done in cpp
	~coutline() { }
	bool IsOutline() { return true; }
	coutline *ToOutline() { return this; }
	int GetTypeBit() { return bitOutline; }
};

/**********************************************************************************************/
/*  RELATED TO cnet2                                                                           */
/**********************************************************************************************/

// cnet2: describes a net
class cnet2: public cpcb_item
{
	friend class CNetList;

public:
	carray<cconnect2> connects;	// array of pointers to connections.  Type change
	CString name;				// net name
	carray<cpin2> pins;			// array of pins
	carray<carea2> areas;		// array of copper areas
	int def_w;					// default trace width
	int def_via_w;				// default via width
	int def_via_hole_w;			// default via hole width
	bool bVisible;				// FALSE to hide ratlines and make unselectable.  CPT2 TODO put in base class?
	int utility2;				// used to keep track of which nets have been optimized
	// CDisplayList * m_dlist;		// CDisplayList to use	// CPT2 use doc->m_dlist (?)
	cnetlist * m_nlist;			// parent netlist

	cnet2( cnetlist *nlist, CString _name, int _def_w, int _def_via_w, int _def_via_hole_w );			// Done in cpp
	~cnet2()
	{
		citer<cpin2> ip (&pins);
		for (cpin2 *p = ip.First(); p; p = ip.Next())
			p->net = NULL;
	}

	bool IsNet() { return true; }
	cnet2 *ToNet() { return this; }
	int GetTypeBit() { return bitNet; }

	// general
	void GetStatusStr( CString * str );
	bool GetVisible() { return bVisible; }

	void SetVisible( bool _bVisible );				// Done in cpp
	void HighlightConnections()						// CPT2 TODO Derive from CNetList::HighlightNetConnections().  
													// ?Not implementing exclude_id arg at this point
	{
	}

	void Highlight();												// Done in cpp
	void CalcViaWidths(int w, int *via_w, int *via_hole_w);			// Done in cpp
	// pins
	int NumPins() { return pins.GetNItems(); }
	// cpin2 * PinByIndex( int ip );
	// ? cpin2 * PinByUID( int uid, int * index );
	void SetVertexToPin( cvertex2 *v, cpin2 *p );					// Arg change
	int TestHitOnAnyPad( int x, int y, int layer );					// CPT2 TODO adapt from CNetList function
	// connections
	int NumCons() { return connects.GetNItems(); }
	// cconnect2 * ConByIndex( int ic );
	// cconnect2 * ConByUID( int uid, int * index=NULL );
	// int ConIndexByPtr( cconnect2 * c );

	// copper areas
	int NumAreas() { return areas.GetNItems(); }
	// carea2 * AreaByUID( int uid, int * index=NULL );
	// carea2 * AreaByIndex( int ia );
	void SetAreaConnections()
	{
		citer<carea2> ia (&areas);
		for (carea2* a = ia.First(); a; a = ia.Next())
			a->SetConnections();
	}

// methods that edit object
	// pins
	void AddPin( CString * ref_des, CString * pin_name, BOOL bSetAreas=TRUE );
	void RemovePin( cpin2 *pin, BOOL bSetAreas=TRUE );											// CPT2
	// ? void RemovePinByUID( int uid, BOOL bSetAreas=TRUE );
	// void RemovePin( CString * ref_des, CString * pin_name, BOOL bSetAreas=TRUE );			// CPT2 Use above RemovePin, plus:
	cpin2 *GetPinByNames( CString *ref_des, CString *pin_name)
	{
		citer<cpin2> ip (&pins);
		for (cpin2 *p = ip.First(); p; p = ip.Next())
			if (p->part->ref_des == *ref_des && p->pin_name == *pin_name)
				return p;
		return NULL;
	}
	// void RemovePin( int pin_index, BOOL bSetAreas=TRUE );

	void SetWidth( int w, int via_w, int via_hole_w )
	{
		citer<cconnect2> ic (&connects);
		for (cconnect2 *c = ic.First(); c; c = ic.Next())
			c->SetWidth(w, via_w, via_hole_w);
	}

	// connections
	cconnect2 * AddConnect( int * ic=NULL );
	cconnect2 * AddConnectFromPin( int p1, int * ic=NULL );
	cconnect2 * AddConnectFromPinToPin( int p1, int p2, int * ic=NULL );
	cconnect2 * AddConnectFromTraceVtx( id& vtx_id, int * ic=NULL );
	cconnect2 * AddRatlineFromVtxToPin( id vtx_id, int pin_index );
	// void RemoveConnect( cconnect2 * c ); // CPT2 use cconnect::Remove().	Adjusting tees will always happen (why not?)
	// void RemoveConnectAdjustTees( cconnect2 * c ); // CPT2 use cconnect::Remove().
	cconnect2 * SplitConnectAtVtx( id vtx_id );
	void MergeConnections( cconnect2 * c1, cconnect2 * c2 );
	void RecreateConnectFromUndo( undo_con * con, undo_seg * seg, undo_vtx * vtx );
	// void AdjustTees( int tee_ID ); // CPT2 use ctee:Adjust().

	// bool RemoveSegmentAdjustTees( cseg2 * s );		// CPT2 use cseg2::RemoveBreak()
	// bool RemoveSegAndVertexByIndex( cseg2 *s );		// CPT2 use cseg2::RemoveMerge()
	// bool RemoveVertexAndSegByIndex( cseg2 *s );		// CPT2 use cseg2::RemoveMerge()
	// bool RemoveVertex( cconnect2 * c, int iv );		// CPT2 use cvertex2::Remove()
	// bool RemoveVertex( cvertex2 * v );				// Ditto
	// bool MergeUnroutedSegments( cconnect2 * c );		// CPT2 use cconnect::MergeUnroutedSegments()

	void CleanUpConnections( CString * logstr=NULL );				// CPT2 TODO adapt from CNetList::CleanUpConnections(cnet*,CString*)
	int OptimizeConnections(BOOL bBelowPinCount, int pin_count, BOOL bVisibleNetsOnly=TRUE );  // CPT2 TODO:  derive from old CNetList functions
};


/**********************************************************************************************/
/*  OTHERS: ctext, cadhesive, ccentroid                                                       */
/**********************************************************************************************/

class ctext: public cpcb_item
{
public:
	int m_x, m_y;
	int m_layer;
	int m_angle;
	bool m_bMirror;
	bool m_bNegative;
	int m_font_size;
	int m_stroke_width;
	int m_nchars;						// CPT2 TODO. Ditch?
	CString m_str;
	CArray<stroke> m_stroke;
	// CDisplayList * m_dlist;			// CPT2 use doc->m_dlist (?)
	dl_element * dl_sel;
	SMFontUtil * m_smfontutil;


	ctext( CFreePcbDoc *_doc, int _x, int _y, int _angle, 
		BOOL _bMirror, BOOL _bNegative, int _layer, int _font_size, 
		int _stroke_width, SMFontUtil *_smfontutil, CString * _str )			// CPT2 Removed selType/selSubtype args.  Will use derived creftext and cvaluetext
		: cpcb_item (_doc)														// classes in place of this business.
	{
		m_x = _x, m_y = _y;
		m_angle = _angle;
		m_bMirror = _bMirror; m_bNegative = _bNegative;
		m_layer = _layer;
		m_font_size = _font_size;
		m_stroke_width = _stroke_width;
		m_smfontutil = _smfontutil;
		m_str = *_str;
		m_nchars = m_str.GetLength();
	}
	~ctext();

	bool IsText() { return true; }
	ctext *ToText() { return this; }
	int GetTypeBit() { return bitText; }

	void Init( CDisplayList * dlist, int x, int y, int angle,					// TODO: rethink relationship with constructor. Removed tid arg.
		int mirror, BOOL bNegative, int layer, int font_size, 
		int stroke_width, SMFontUtil * smfontutil, CString * str_ptr );
	void Draw( CDisplayList * dlist, SMFontUtil * smfontutil );
	void Draw() { }																// TODO
	void Undraw() { }															// TODO
	void Highlight() { }														// TODO
	void StartDragging( CDC * pDC );
	void CancelDragging();
	void Move( int x, int y, int angle, BOOL mirror, BOOL negative, int layer, int size=-1, int w=-1 );
	void Move( int x, int y, int angle, int size=-1, int w=-1);											// CPT added.  Used when moving ref/value texts
	void GetBounds( CRect &br );
};

class creftext: public ctext
{
public:
	creftext( CFreePcbDoc * doc, int x, int y, int angle, 
		BOOL bMirror, BOOL bNegative, int layer, int font_size, 
		int stroke_width, SMFontUtil * smfontutil, CString * str_ptr ) :
			ctext(doc, x, y, angle, bMirror, bNegative, layer, font_size,
				stroke_width, smfontutil, str_ptr) 
			{ }
	bool IsText() { return false; }
	bool IsRefText() { return true; }
	ctext *ToText() { return NULL; }
	creftext *ToRefText() { return this; }
	int GetTypeBit() { return bitRefText; }
};

class cvaluetext: public ctext
{
public:
	cvaluetext( CFreePcbDoc * doc, int x, int y, int angle, 
		BOOL bMirror, BOOL bNegative, int layer, int font_size, 
		int stroke_width, SMFontUtil * smfontutil, CString * str_ptr ) :
			ctext(doc, x, y, angle, bMirror, bNegative, layer, font_size,
				stroke_width, smfontutil, str_ptr) 
			{ }
	bool IsText() { return false; }
	bool IsValueText() { return true; }
	ctext *ToText() { return NULL; }
	cvaluetext *ToValueText() { return this; }
	int GetTypeBit() { return bitValueText; }
};

class ccentroid : public cpcb_item
{
public:
	// Represents centroids within footprints.
	ccentroid(CFreePcbDoc *_doc) 
		: cpcb_item (_doc)
		{ }
	~ccentroid() { }
	bool IsCentroid() { return true; }
	ccentroid *ToCentroid() { return this; }
	int GetTypeBit() { return bitCentroid; }
};

class cadhesive : public cpcb_item
{
public:
	// Represents adhesive dots within footprints.
	cadhesive(CFreePcbDoc *_doc) 
		: cpcb_item (_doc)
		{ }
	~cadhesive() { }
	bool IsAdhesive() { return true; }
	cadhesive *ToAdhesive() { return this; }
	int GetTypeBit() { return bitAdhesive; }
};

class cdrc : public cpcb_item
{
public:
	// Represents Design Rule Check error items.  Incorporates old class DRError
	enum {				
		// subtypes of errors 
		PAD_PAD = 1,				
		PAD_PADHOLE,		
		PADHOLE_PADHOLE,	
		SEG_PAD,			
		SEG_PADHOLE,	
		VIA_PAD,			
		VIA_PADHOLE,		
		VIAHOLE_PAD,		
		VIAHOLE_PADHOLE,	
		SEG_SEG,		
		SEG_VIA,			
		SEG_VIAHOLE,	
		VIA_VIA,			
		VIA_VIAHOLE,		
		VIAHOLE_VIAHOLE,	
		TRACE_WIDTH,	
		RING_PAD,			
		RING_VIA,
		BOARDEDGE_PAD,			
		BOARDEDGE_PADHOLE,	
		BOARDEDGE_VIA,			
		BOARDEDGE_VIAHOLE,		
		BOARDEDGE_SEG,	
		BOARDEDGE_COPPERAREA,	
		COPPERGRAPHIC_PAD,			
		COPPERGRAPHIC_PADHOLE,	
		COPPERGRAPHIC_VIA,			
		COPPERGRAPHIC_VIAHOLE,		
		COPPERGRAPHIC_SEG,	
		COPPERGRAPHIC_COPPERAREA,	
		COPPERGRAPHIC_BOARDEDGE,	
		COPPERAREA_COPPERAREA,
		COPPERAREA_INSIDE_COPPERAREA,
		COPPERAREA_BROKEN,					// CPT
		UNROUTED
	};

	int layer;					// layer (if pad error)
	int type;					// id, using subtypes above
	CString str;				// descriptive string
	CString name1, name2;		// names of nets or parts tested
	cpcb_item *item1, *item2;	// ids of items tested
	int x, y;					// position of error

	cdrc(CFreePcbDoc *_doc) 
		: cpcb_item(_doc)
		{ }
	~cdrc() { }
	bool IsDRC() { return true; }
	cdrc *ToDRC() { return this; }
	int GetTypeBit() { return bitDRC; }
};

