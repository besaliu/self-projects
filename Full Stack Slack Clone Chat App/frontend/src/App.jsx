import {useState, createContext, useEffect} from 'react';
import ButtonAppBar from './AppBar';
import Login from './Login';
import Channels from './Channel';
import LogoutIcon from '@mui/icons-material/Logout';
import HomeIcon from '@mui/icons-material/Home';
import Button from '@mui/material/Button';
import Container from '@mui/material/Container';
import Box from '@mui/material/Box';
import {authenticatedFetch} from './utils';

export const pageContext = createContext();
export const userContext = createContext();

/**
 *
 * @returns {any} any
 */
function App() {
  const [user, setUser] = useState('none');
  const [workspace, setWorkspace] = useState(null);
  const [isChannelView, setIsChannelView] = useState(false);

  const selectWorkspace = (workspaceName) => {
    setWorkspace(workspaceName);
    setIsChannelView(false);
  };

  const toggleSelected = async (user) => {
    setUser(user);
    const response = await authenticatedFetch(`http://localhost:3010/api/v0/workspace?username=${user}`);
    const workspaces = await response.json();
    if (workspaces.length > 0) {
      setWorkspace(workspaces[0].name);
    }
  };

  const goToWorkspace = () => {
    window.dispatchEvent(new CustomEvent('returnToChannelsList'));
  };

  useEffect(() => {
    // Listen for changes in view (channels list or messages)
    const handleViewChange = (event) => {
      setIsChannelView(event.detail.isChannelView);
    };

    window.addEventListener('viewChanged', handleViewChange);

    return () => {
      window.removeEventListener('viewChanged', handleViewChange);
    };
  }, []);

  return (
    <Container sx={{
      padding: 0,
      height: '100vh',
      width: '100vw',
      maxWidth: 'none',
      display: 'flex',
      flexDirection: 'column',
    }}>
      {user === 'none' ? (
        <Login toggleSelected={toggleSelected} />
      ) : (
        <Box sx={{display: 'flex', flexDirection: 'column', height: '100%'}}>
          <userContext.Provider value={user}>
            <pageContext.Provider value={workspace}>
              <ButtonAppBar selectedPage={selectWorkspace} />
            </pageContext.Provider>
          </userContext.Provider>

          <Box
            sx={{
              flexGrow: 1,
              overflow: 'auto',
              display: 'flex',
              flexDirection: 'column',
            }}
          >
            <pageContext.Provider value={workspace}>
              <Channels />
            </pageContext.Provider>
          </Box>

          <Box
            sx={{
              position: 'fixed',
              bottom: 16,
              left: 16,
              zIndex: 10,
            }}
          >
            <Button
              variant="contained"
              color="primary"
              aria-label="home"
              onClick={goToWorkspace}
              startIcon={<HomeIcon />}
              disabled={!isChannelView}
              sx={{
                opacity: !isChannelView ? 0.5 : 1,
              }}
            >
            </Button>
          </Box>

          <Box
            sx={{
              position: 'fixed',
              bottom: 16,
              right: 16,
              zIndex: 10,
            }}
          >
            <Button
              variant="contained"
              color="error"
              aria-label="logout"
              onClick={() => setUser('none')}
              startIcon={<LogoutIcon />}
            >
            </Button>
          </Box>
        </Box>
      )}
    </Container>
  );
}

export default App;
