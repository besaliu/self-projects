import * as React from 'react';
import AppBar from '@mui/material/AppBar';
import Box from '@mui/material/Box';
import Toolbar from '@mui/material/Toolbar';
import Typography from '@mui/material/Typography';
import Button from '@mui/material/Button';
import Menu from '@mui/material/Menu';
import MenuItem from '@mui/material/MenuItem';
import ArrowBackIcon from '@mui/icons-material/ArrowBack';
import ArrowDropDownCircleIcon from '@mui/icons-material/ArrowDropDownCircle';
import {useContext, useState, useEffect} from 'react';
import {pageContext, userContext} from './App';
import PropTypes from 'prop-types';
import {authenticatedFetch} from './utils';

/**
 *
 * @param {any} root0 root
 * @param {any} root0.selectedPage root
 * @returns {any} any
 */
function BasicMenu({selectedPage}) {
  const user = useContext(userContext);
  const [anchorEl, setAnchorEl] = React.useState(null);
  const open = Boolean(anchorEl);
  const [workspaces, setWorkspaces] = useState([]);

  const fetchWorkspaces = async () => {
    const response = await authenticatedFetch(`http://localhost:3010/api/v0/workspace?username=${user}`);
    const data = await response.json();

    // For each workspace, check if current user is the owner
    const workspacesWithOwnership = await Promise.all(
        data.map(async (workspace) => {
          const ownerResponse = await authenticatedFetch(
              `http://localhost:3010/api/v0/workspace/owner?workspace=${workspace.name}`,
          );

          if (ownerResponse.ok) {
            const ownerData = await ownerResponse.json();
            const isOwner = ownerData.name === user;
            return {...workspace, isOwner};
          }
          return {...workspace, isOwner: false};
        }));

    setWorkspaces(workspacesWithOwnership);
  };

  const handleClick = (event) => {
    setAnchorEl(event.currentTarget);
    fetchWorkspaces();
  };

  const handleClose = () => {
    setAnchorEl(null);
  };

  const handleWorkspaceSelect = (workspace) => {
    selectedPage(workspace);
    handleClose();
  };

  return (
    <Box>
      <Button
        aria-label='workspaces'
        aria-controls={open ? 'basic-menu' : undefined}
        aria-haspopup="true"
        aria-expanded={open ? 'true' : undefined}
        onClick={handleClick}
        color="inherit"
        variant="text"
        sx={{textTransform: 'none'}}
        startIcon={<ArrowDropDownCircleIcon />}
      >
      </Button>
      <Menu
        aria-label='workspace-menu'
        anchorEl={anchorEl}
        open={open}
        onClose={handleClose}
        MenuListProps={{
          'aria-labelledby': 'basic-button',
        }}
        sx={{
          'mt': 1,
          '& .MuiPaper-root': {
            minWidth: 200,
            maxHeight: 300,
            overflowY: 'auto',
          },
        }}
        transformOrigin={{
          vertical: 'top',
          horizontal: 'right',
        }}
        anchorOrigin={{
          vertical: 'bottom',
          horizontal: 'right',
        }}
      >
        {workspaces.map((workspace) => (
          <MenuItem
            key={workspace.workspace_id}
            onClick={() => handleWorkspaceSelect(workspace.name)}
            sx={{
              display: 'flex',
              justifyContent: 'space-between',
              alignItems: 'center',
            }}
          >
            <span>{workspace.name}</span>
            {workspace.isOwner && (
              <Box
                sx={{
                  ml: 1,
                  width: '20px',
                  height: '20px',
                  borderRadius: '50%',
                  backgroundColor: 'primary.main',
                  color: 'white',
                  display: 'flex',
                  alignItems: 'center',
                  justifyContent: 'center',
                  fontSize: '12px',
                  fontWeight: 'bold',
                }}
              >
        O
              </Box>
            )}
          </MenuItem>
        ))}
      </Menu>
    </Box>
  );
}

/**
 *
 * @param {any} root0 root
 * @param {any} root0.selectedPage root
 * @returns {any} any
 */
export default function ButtonAppBar({selectedPage}) {
  const workspace = useContext(pageContext);
  const [displayTitle, setDisplayTitle] = useState('');
  const [isChannelView, setIsChannelView] = useState(false);

  useEffect(() => {
    if (workspace) {
      setDisplayTitle(workspace);
      setIsChannelView(false);
    }
  }, [workspace]);

  useEffect(() => {
    // Listen for title updates from Channel component
    const handleTitleUpdate = (event) => {
      setDisplayTitle(event.detail.title);
      setIsChannelView(event.detail.type === 'channel');
    };

    window.addEventListener('updateAppBarTitle', handleTitleUpdate);

    return () => {
      window.removeEventListener('updateAppBarTitle', handleTitleUpdate);
    };
  }, []);

  return (
    <Box sx={{flexGrow: 0}}>
      <AppBar position="static" color="primary">
        <Toolbar>
          {isChannelView && (
            <Button
              color="inherit"
              aria-label='back'
              aria-hidden='false'
              onClick={() => {
                window.dispatchEvent(new CustomEvent('appBarBackPress'));
              }}
              sx={{mr: 1, minWidth: 'auto', p: 1}}
            >
              <ArrowBackIcon />
            </Button>
          )}
          <Typography
            variant="h6"
            component="div"
            sx={{
              flexGrow: 1,
              fontWeight: 'medium',
            }}
          >
            {displayTitle}
          </Typography>
          <BasicMenu selectedPage={selectedPage} />
        </Toolbar>
      </AppBar>
    </Box>
  );
}

ButtonAppBar.propTypes = {
  selectedPage: PropTypes.func.isRequired,
};

BasicMenu.propTypes = {
  selectedPage: PropTypes.func.isRequired,
};
