// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once


// Enum holding the state of the Friends manager
namespace EFriendsAndManagerState
{
	enum Type
	{
		Idle,								// Idle - can accept requests
		RequestingFriendsList,				// Requesting a list refresh
		RequestFriendsListRefresh,			// List request in progress
		ProcessFriendsList,					// Process the Friends List after a list refresh
		RequestingFriendName,				// Requesting a friend add
		DeletingFriends,					// Deleting a friend
		AcceptingFriendRequest,				// Accepting a friend request
	};
};


/** Delegate type for FriendsList updated. */
DECLARE_MULTICAST_DELEGATE(FOnFriendsUpdated)


/**
 * Implement the Friend and Chat manager
 */
class FFriendsAndChatManager
	: public TSharedFromThis<FFriendsAndChatManager>
{
public:

	/** Default constructor. */
	FFriendsAndChatManager( );
	
	/** Destructor. */
	~FFriendsAndChatManager( );

public:

	/** */
	void StartupManager();

	/**
	 * Init the manager.
	 *
	 * @param Notification delegate The delegate for message notifications.
	 */
	void Init(FOnFriendsNotification& NotificationDelegate);

	/** Logout and close any Friends windows. */
	void Logout();

	/**
	 * Create the friends list window.
	 *
	 * @param ParentWindow The holding window.
	 * @param InStyle The style used to create the widgets.
	 */
	void GenerateFriendsWindow( TSharedPtr< const SWidget > ParentWidget, const FFriendsAndChatStyle* InStyle );

	/**
	 * Set if the player is in a session.
	 *
	 * @param bInSession Whether we are in a game session.
	 */
	void SetInSession( bool bInSession );

	/**
	 * Get if the player is in a session.
	 *
	 * @return True if we are in a game session.
	 */
	bool IsInSession();

	/**
	 * Create the friends list widget without a holder
	 *
	 * @param InStyle The style used to create the widgets.
	 * @return The generated widget.
	 */
	TSharedPtr< SWidget > GenerateFriendsListWidget( const FFriendsAndChatStyle* InStyle );

	/**
	 * Create the friends list window.
	 *
	 * @param FriendItem The friend to start a chat with.
	 */
	void GenerateChatWindow( TSharedPtr< FFriendStuct > FriendItem );

	/**
	 * Accept a friend request.
	 *
	 * @param FriendItem The friend to accept.
	 */
	void AcceptFriend( TSharedPtr< FFriendStuct > FriendItem );

	/**
	 * Reject a friend request.
	 *
	 * @param FriendItem The friend to reject.
	 */
	void RejectFriend( TSharedPtr< FFriendStuct > FriendItem );

	/**
	 * Clear any existing notifications
	 *
	 * @param FriendItem The friend related to the notification message.
	 */
	void ClearNotification( TSharedPtr< FFriendStuct > FriendItem );

	/**
	 * Get the friends filtered list of friends.
	 *
	 * @param OutFriendsList  Array of friends to fill in.
	 * @return the friend list count.
	 */
	int32 GetFilteredFriendsList( TArray< TSharedPtr< FFriendStuct > >& OutFriendsList );

	/**
	 * Get outgoing request list.
	 *
	 * @param OutFriendsList  Array of friends to fill in.
	 * @return The friend list count.
	 */
	int32 GetFilteredOutgoingFriendsList( TArray< TSharedPtr< FFriendStuct > >& OutFriendsList );

	/**
	 * Request a friend be added.
	 *
	 * @param FriendName The friend name.
	 */
	void RequestFriend( const FText& FriendName );

	/**
	 * Request a friend to be deleted.
	 *
	 * @param FriendItem The friend item to delete.
	 */
	void DeleteFriend( TSharedPtr< FFriendStuct > FriendItem );

	/**
	 * Accessor for the Friends List updated delegate.
	 *
	 * @return The delegate.
	 */
	FOnFriendsUpdated& OnFriendsListUpdated()
	{
		return OnFriendsListUpdatedDelegate;
	}

	/**
	 * Get the manager state.
	 *
	 * @return The manager state.
	 */
	EFriendsAndManagerState::Type GetManagerState();

	/**
	 * Find a user ID.
	 *
	 * @param InUserName The user name to find.
	 * @return The unique ID.
	 */
	TSharedPtr< FUniqueNetId > FindUserID( const FString& InUsername );

	/**
	 * Is this user pending an invite.
	 *
	 * @param InUserName The user name to find.
	 * @return True if pending an invite.
	 */	
	bool IsPendingInvite( const FString& InUsername );

	/**
	 * Find a user.
	 *
	 * @param InUserName The user name to find.
	 * @return The Friend ID.
	 */
	TSharedPtr< FFriendStuct > FindUser( TSharedRef<FUniqueNetId> InUserID);

private:

	/** Process the friends list to send out invites */
	void SendFriendRequests();

	/** Refresh the data lists */
	void RefreshList();

	/** Build the friends list used in the UI. */
	void BuildFriendsList();

	/** Build the friends UI. */
	void BuildFriendsUI();

	/**
	 * Set the manager state.
	 *
	 * @param NewState The new manager state.
	 */
	void SetState( EFriendsAndManagerState::Type NewState );

	/** Send a friend invite notification. */
	void SendFriendInviteNotification();

	/** Called when singleton is released. */
	void ShutdownManager();

	/**
	 * Delegate used when the friends read request has completed.
	 *
	 * @param LocalPlayer		The controller number of the associated user that made the request.
	 * @param bWasSuccessful	true if the async action completed without error, false if there was an error.
	 * @param ListName			Name of the friends list that was operated on.
	 * @param ErrorStr			String representing the error condition.
	 */
	void OnReadFriendsListComplete( int32 LocalPlayer, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr );

	/**
	 * Delegate used when an invite send request has completed.
	 *
	 * @param LocalPlayer		The controller number of the associated user that made the request.
	 * @param bWasSuccessful	true if the async action completed without error, false if there was an error.
	 * @param FriendId			Player that was invited.
	 * @param ListName			Name of the friends list that was operated on.
	 * @param ErrorStr			String representing the error condition.
	 */
	void OnSendInviteComplete( int32 LocalPlayer, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& ListName, const FString& ErrorStr );

	/**
	 * Delegate used when an invite accept request has completed.
	 *
	 * @param LocalPlayer		The controller number of the associated user that made the request
	 * @param bWasSuccessful	true if the async action completed without error, false if there was an error
	 * @param FriendId			Player that invited us
	 * @param ListName			Name of the friends list that was operated on
	 * @param ErrorStr			String representing the error condition
	 */
	void OnAcceptInviteComplete( int32 LocalPlayer, bool bWasSuccessful, const FUniqueNetId& FriendId, const FString& ListName, const FString& ErrorStr );

	/**
	 * Delegate used when the friends delete request has completed.
	 *
	 * @param LocalPlayer		The controller number of the associated user that made the request.
	 * @param bWasSuccessful	true if the async action completed without error, false if there was an error.
	 * @param DeletedFriendID	The ID of the deleted friend.
	 * @param ListName			Name of the friends list that was operated on.
	 * @param ErrorStr			String representing the error condition.
	 */
	void OnDeleteFriendComplete( int32 LocalPlayer, bool bWasSuccessful, const FUniqueNetId& DeletedFriendID, const FString& ListName, const FString& ErrorStr );

	/**
	 * Delegate used when a friend ID request has completed.
	 *
	 * @param bWasSuccessful		true if the async action completed without error, false if there was an error.
	 * @param RequestingUserId		The ID of the user making the request.
	 * @param DisplayName			The string ID of the friend found.
	 * @param IdentifiedUserId		The net ID of the found friend.
	 * @param ErrorStr				String representing the error condition.
	 */
	void OnQueryUserIdFromDisplayNameComplete(bool bWasSuccessful, const FUniqueNetId& RequestingUserId, const FString& DisplayName, const FUniqueNetId& IdentifiedUserId, const FString& Error);

	/**
	 * Delegate used when a query user info request has completed.
	 *
	 * @param LocalPlayer		The controller number of the associated user that made the request.
	 * @param bWasSuccessful	true if the async action completed without error, false if there was an error.
	 * @param UserIds			An array of user IDs filled out with info.
	 * @param ErrorStr			String representing the error condition.
	 */
	void OnQueryUserInfoComplete( int32 LocalPlayer, bool bWasSuccessful, const TArray< TSharedRef<class FUniqueNetId> >& UserIds, const FString& ErrorStr );

	/**
	 * Handle an accept message accepted from a notification.
	 *
	 * @param MessageNotification The message responded to.
 	 * @param ResponseTpe The response from the message request.
	 */
	FReply HandleMessageAccepted( TSharedPtr< FFriendsAndChatMessage > MessageNotification, EFriendsResponseType::Type ResponseType );

	/**
	 * Validate friend list after a refresh. Check if the list has changed.
	 *
	 * @return True if the list has changes and the UI needs refreshing.
	 */
	bool UpdateFriendsList();

	/**
	 * A ticker used to perform updates on the main thread.
	 *
	 * @param Delta The tick delta.
	 * @return true to continue ticking.
	 */
	bool Tick( float Delta );

	// UI functions

	/** Close button clicked. */
	FReply OnCloseClicked();

	/** Minimize button clicked. */
	FReply OnMinimizeClicked();

	/** Close button clicked. */
	FReply OnChatCloseClicked();

	/** Minimize button clicked. */
	FReply OnChatMinimizeClicked();

private:

	/* Lists used to keep track of Friends
	*****************************************************************************/

	// Holds the list of user names to query add as friends 
	TArray< FString > FriendByNameRequests;
	// Holds the list of user names to send invites to. 
	TArray< FString > FriendByNameInvites;
	// Holds the list of Unique Ids found for user names to add as friends
	TArray< TSharedRef< FUniqueNetId > > QueryUserIds;
	// Holds the full friends list used to build the UI
	TArray< TSharedPtr< FFriendStuct > > FriendsList;
	// Holds the filtered friends list used in the UI
	TArray< TSharedPtr< FFriendStuct > > FilteredFriendsList;
	// Holds the outgoing friend request list used in the UI
	TArray< TSharedPtr< FFriendStuct > > FilteredOutgoingList;
	// Holds the unprocessed friends list generated from a friends request update
	TArray< TSharedPtr< FFriendStuct > > PendingFriendsList;
	// Holds the list of incoming invites that need to be responded to
	TArray< TSharedPtr< FFriendStuct > > PendingIncomingInvitesList;
	// Holds the list of invites we have already responded to
	TArray< TSharedPtr< FUniqueNetId > > NotifiedRequest;
	// Holds the list messages sent out to be responded to
	TArray<TSharedPtr< FFriendsAndChatMessage > > NotificationMessages;
	// Holds an array of outgoing invite friend requests
	TArray< TSharedRef< FUniqueNetId > > PendingOutgoingFriendRequests;
	// Holds an array of outgoing delete friend requests
	TArray< FUniqueNetIdString > PendingOutgoingDeleteFriendRequests;
	// Holds an array of outgoing accept friend requests
	TArray< FUniqueNetIdString > PendingOutgoingAcceptFriendRequests;

	/* Delegates
	*****************************************************************************/

	// Holds the ticker delegate
	FTickerDelegate UpdateFriendsTickerDelegate;
	// Delegate when friend list has been read
	FOnReadFriendsListCompleteDelegate OnReadFriendsCompleteDelegate;
	// Delegate when invites accepted
	FOnAcceptInviteCompleteDelegate OnAcceptInviteCompleteDelegate;
	// Delegate when invites have been sent
	FOnSendInviteCompleteDelegate OnSendInviteCompleteDelegate;
	// Delegate to use for deleting a friend
	FOnDeleteFriendCompleteDelegate OnDeleteFriendCompleteDelegate;
	// Delegate for querying user id from a name string
	FOnQueryUserIdFromDisplayNameCompleteDelegate OnQueryUserIdFromDisplayNameCompleteDelegate;
	// Delegate to use for querying user info list
	FOnQueryUserInfoCompleteDelegate OnQueryUserInfoCompleteDelegate;
	// Holds the delegate to call when the friends list gets updated - refresh the UI
	FOnFriendsUpdated OnFriendsListUpdatedDelegate;
	// Holds the Friends list notification delegate
	FOnFriendsNotification* FriendsListNotificationDelegate;

	/* Identity stuff
	*****************************************************************************/

	// Holds the Online identity
	IOnlineIdentityPtr OnlineIdentity;
	// Holds the Friends Interface
	IOnlineFriendsPtr FriendsInterface;
	// Holds the Online Subsystem
	FOnlineSubsystemMcp* OnlineSubMcp;

	/* Manger state
	*****************************************************************************/

	// Holds the manager state
	EFriendsAndManagerState::Type ManagerState;
	// Holds the update interval
	const float UpdateTimeInterval;
	// Holds the update timer
	float UpdateTimer;

	/* UI
	*****************************************************************************/
	// Holds the parent window
	TSharedPtr< const SWidget > ParentWidget;
	// Holds the main Friends List window
	TSharedPtr< SWindow > FriendWindow;
	// Holds the Friends List widget
	TSharedPtr< SWidget > FriendListWidget;
	// Holds the chat window
	TSharedPtr< SWindow > ChatWindow;
	// Holds the style used to create the Friends List widget
	FFriendsAndChatStyle Style;
	// Holds if the game is in session
	bool bIsInSession;
	// Holds if the Friends list is inited
	bool bIsInited;

public:

	static TSharedRef< FFriendsAndChatManager > Get();
	static void Shutdown();

private:

	static TSharedPtr< FFriendsAndChatManager > SingletonInstance;
};
