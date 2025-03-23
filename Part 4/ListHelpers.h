#pragma once

#include <Windows.h>

    /// <summary>
    /// The InitializeListHead routine initializes a LIST_ENTRY structure that represents the head of a doubly linked list
    /// </summary>
    /// <param name="ListHead">Pointer to a LIST_ENTRY structure that serves as the list header</param>
    FORCEINLINE
    VOID
    InitializeListHead(
        _Out_ PLIST_ENTRY ListHead
    )
{
    ListHead->Flink = ListHead->Blink = ListHead;
    return;
}



/// <summary>
/// The IsListEmpty routine indicates whether a doubly linked list of LIST_ENTRY structures is empty
/// </summary>
/// <param name="ListHead">Pointer to a LIST_ENTRY structure that represents the head of the list</param>
/// <returns>IsListEmpty returns TRUE if there are currently no entries in the list and FALSE otherwise</returns>
_Must_inspect_result_
BOOLEAN
CFORCEINLINE
IsListEmpty(
    _In_ const LIST_ENTRY * ListHead
)
{
    return (BOOLEAN)(ListHead->Flink == ListHead);
}


/// <summary>
/// The RemoveEntryList routine removes an entry from a doubly linked list of LIST_ENTRY structures
/// </summary>
/// <param name="Entry">Pointer to the LIST_ENTRY structure that represents the entry to be removed</param>
/// <returns>RemoveEntryList returns TRUE if, after removal of the designated entry, the list is empty. Otherwise, the routine returns FALSE to indicate that the resulting list still contains one or more entries</returns>
FORCEINLINE
BOOLEAN
RemoveEntryList(
    _In_ PLIST_ENTRY Entry
)
{
    PLIST_ENTRY Blink;
    PLIST_ENTRY Flink;

    Flink = Entry->Flink;
    Blink = Entry->Blink;
    Blink->Flink = Flink;
    Flink->Blink = Blink;
    return (BOOLEAN)(Flink == Blink);
}


/// <summary>
/// The RemoveHeadList routine removes an entry from the beginning of a doubly linked list of LIST_ENTRY structures
/// </summary>
/// <param name="ListHead">Pointer to the LIST_ENTRY structure that serves as the list header</param>
/// <returns>RemoveHeadList returns a pointer to the entry removed from the list. If the list is empty, RemoveHeadList returns ListHead</returns>
FORCEINLINE
PLIST_ENTRY
RemoveHeadList(
    _Inout_ PLIST_ENTRY ListHead
)
{
    PLIST_ENTRY Flink;
    PLIST_ENTRY Entry;

    Entry = ListHead->Flink;
    Flink = Entry->Flink;
    ListHead->Flink = Flink;
    Flink->Blink = ListHead;
    return Entry;
}


/// <summary>
/// The RemoveTailList routine removes an entry from the end of a doubly linked list of LIST_ENTRY structures
/// </summary>
/// <param name="ListHead">Pointer to the LIST_ENTRY structure that serves as the list header</param>
/// <returns>RemoveTailList returns a pointer to the entry that was at the tail of the list. If the list is empty, RemoveTailList returns ListHead</returns>
FORCEINLINE
PLIST_ENTRY
RemoveTailList(
    _Inout_ PLIST_ENTRY ListHead
)
{
    PLIST_ENTRY Blink;
    PLIST_ENTRY Entry;

    Entry = ListHead->Blink;
    Blink = Entry->Blink;
    ListHead->Blink = Blink;
    Blink->Flink = ListHead;
    return Entry;
}


/// <summary>
/// The InsertTailList routine inserts an entry at the tail of a doubly linked list of LIST_ENTRY structures
/// </summary>
/// <param name="ListHead">Pointer to the LIST_ENTRY structure that represents the head of the list</param>
/// <param name="Entry">Pointer to a LIST_ENTRY structure that represents the entry to be inserted in the list</param>
FORCEINLINE
VOID
InsertTailList(
    _Inout_ PLIST_ENTRY ListHead,
    _Inout_ PLIST_ENTRY Entry
)
{
    PLIST_ENTRY Blink;

    Blink = ListHead->Blink;
    Entry->Flink = ListHead;
    Entry->Blink = Blink;
    Blink->Flink = Entry;
    ListHead->Blink = Entry;
    return;
}


/// <summary>
/// The InsertHeadList routine inserts an entry at the head of a doubly linked list of LIST_ENTRY structures
/// </summary>
/// <param name="ListHead">Pointer to the LIST_ENTRY structure that represents the head of the list</param>
/// <param name="Entry">Pointer to a LIST_ENTRY structure that represents the entry to be inserted into the list</param>
FORCEINLINE
VOID
InsertHeadList(
    _Inout_ PLIST_ENTRY ListHead,
    _Inout_ PLIST_ENTRY Entry
)
{
    PLIST_ENTRY Flink;

    Flink = ListHead->Flink;
    Entry->Flink = Flink;
    Entry->Blink = ListHead;
    Flink->Blink = Entry;
    ListHead->Flink = Entry;
    return;
}