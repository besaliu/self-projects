import {useState, useEffect, useContext} from 'react';
import {pageContext} from './App';
import Typography from '@mui/material/Typography';
import List from '@mui/material/List';
import ListItem from '@mui/material/ListItem';
import ListItemText from '@mui/material/ListItemText';
import Box from '@mui/material/Box';
import Divider from '@mui/material/Divider';
import {authenticatedFetch} from './utils';

/**
 * @returns {any} channels
 */
export default function Channels() {
  const workspace = useContext(pageContext);
  const [channels, setChannels] = useState([]);
  const [messages, setMessages] = useState([]);
  const [view, setView] = useState('channels');

  const updateView = (newView) => {
    setView(newView);

    window.dispatchEvent(new CustomEvent('viewChanged', {
      detail: {isChannelView: newView === 'messages'},
    }));
  };

  useEffect(() => {
    if (workspace) {
      fetchChannelData();
      setMessages([]);
      updateView('channels');
    }
  }, [workspace]);

  useEffect(() => {
    // Handle back button press from AppBar
    const handleBackPress = () => {
      if (view === 'messages') {
        // If in message view, go back to channels view
        updateView('channels');
        // Update app bar title to show workspace name
        window.dispatchEvent(new CustomEvent('updateAppBarTitle', {
          detail: {title: workspace, type: 'workspace'},
        }));
      }
    };

    window.addEventListener('appBarBackPress', handleBackPress);

    // Handle home button press
    const handleReturnToChannelsList = () => {
      if (view === 'messages') {
        updateView('channels');
        // Update app bar title to show workspace name
        window.dispatchEvent(new CustomEvent('updateAppBarTitle', {
          detail: {title: workspace, type: 'workspace'},
        }));
      }
    };

    window.addEventListener('returnToChannelsList', handleReturnToChannelsList);

    return () => {
      window.removeEventListener('appBarBackPress', handleBackPress);
      window.removeEventListener(
          'returnToChannelsList', handleReturnToChannelsList);
    };
  }, [view, workspace]);

  const fetchChannelData = async () => {
    const response = await authenticatedFetch(`http://localhost:3010/api/v0/channel?workspace=${workspace}`);
    const data = await response.json();
    setChannels(data);
  };

  const fetchChannelMessages = async (channelId) => {
    const response = await authenticatedFetch(`http://localhost:3010/api/v0/channel/${channelId}`);
    const data = await response.json();
    setMessages(data.messages || []);
  };

  const handleChannelSelect = (channel) => {
    fetchChannelMessages(channel.id);
    updateView('messages');

    window.dispatchEvent(new CustomEvent('updateAppBarTitle', {
      detail: {title: channel.name, type: 'channel'},
    }));
  };

  const formatTimestamp = (timestamp) => {
    const date = new Date(timestamp);
    return date.toLocaleTimeString([], {hour: '2-digit', minute: '2-digit'});
  };

  const formatDate = (timestamp) => {
    const date = new Date(timestamp);
    const today = new Date();
    const yesterday = new Date(today);
    yesterday.setDate(yesterday.getDate() - 1);

    if (date.toDateString() === today.toDateString()) {
      return 'Today';
    } else if (date.toDateString() === yesterday.toDateString()) {
      return 'Yesterday';
    } else {
      const options = {weekday: 'long', month: 'long', day: 'numeric'};
      return date.toLocaleDateString(undefined, options);
    }
  };

  // Group messages by date
  const groupMessagesByDate = (messages) => {
    const groups = {};

    messages.forEach((message) => {
      const date = new Date(message.timestamp);
      const dateString = date.toDateString();

      if (!groups[dateString]) {
        groups[dateString] = [];
      }

      groups[dateString].push(message);
    });

    // Sort dates in ascending order (oldest first)
    return Object.entries(groups)
        .sort(([dateA], [dateB]) => new Date(dateA) - new Date(dateB))
        .map(([date, msgs]) => ({
          date,
          displayDate: formatDate(new Date(date)),
          messages: msgs.sort((a, b) => new Date(
              a.timestamp) - new Date(b.timestamp)),
        }));
  };


  // Channel list view
  if (view === 'channels') {
    return (
      <Box sx={{height: '100%', overflow: 'auto'}}>
        <Typography variant="h6" sx={{p: 2, borderBottom: '1px solid #eee'}}>
          Channels
        </Typography>

        {channels.length > 0 ? (
          <List>
            {channels.map((channel) => (
              <ListItem
                key={channel.id}
                onClick={() => handleChannelSelect(channel)}
                divider
                sx={{cursor: 'pointer'}}
              >
                <ListItemText primary={channel.name} />
              </ListItem>
            ))}
          </List>
        ) : (
          <Box sx={{p: 2}}>
            <Typography variant="body1" color="text.secondary">
              No channels available
            </Typography>
          </Box>
        )}
      </Box>
    );
  }

  // Message view with date grouping
  const groupedMessages = groupMessagesByDate(messages);

  return (
    <Box sx={{display: 'flex', flexDirection: 'column', height: '100%'}}>
      <Box sx={{flexGrow: 1, overflow: 'auto', p: 2}}>
        {groupedMessages.length > 0 ? (
          groupedMessages.map((group) => (
            <Box key={group.date} sx={{mb: 3}}>
              <Box
                sx={{
                  position: 'relative',
                  textAlign: 'center',
                  mb: 2,
                }}
              >
                <Divider sx={{my: 2}} />
                <Typography
                  variant="subtitle2"
                  component="span"
                  sx={{
                    position: 'absolute',
                    top: '50%',
                    left: '50%',
                    transform: 'translate(-50%, -50%)',
                    bgcolor: 'background.paper',
                    px: 2,
                    py: 0.5,
                    border: '1px solid #ddd',
                    borderRadius: 1,
                  }}
                >
                  {group.displayDate}
                </Typography>
              </Box>

              {group.messages.map((message) => (
                <Box
                  key={message.id}
                  sx={{
                    display: 'flex',
                    mb: 2,
                  }}
                >
                  <Box sx={{flexGrow: 1}}>
                    <Box sx={{display: 'flex', alignItems: 'center', mb: 0.5}}>
                      <Typography variant="subtitle2"
                        sx={{fontWeight: 'bold', mr: 1}}>
                        {message.username}
                      </Typography>
                      <Typography variant="caption" color="text.secondary">
                        {formatTimestamp(message.timestamp)}
                      </Typography>
                    </Box>
                    <Typography variant="body1" sx={{mb: 1,
                      whiteSpace: 'pre-wrap'}}>
                      {message.content}
                    </Typography>
                  </Box>
                </Box>
              ))}
            </Box>
          ))
        ) : (
          <Typography variant="body1" color="text.secondary"
            sx={{textAlign: 'center', mt: 4}}>
            No messages in this channel
          </Typography>
        )}
      </Box>
    </Box>
  );
}
