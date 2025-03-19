import React from 'react';
import PropTypes from 'prop-types';
import Paper from '@mui/material/Paper';
import Typography from '@mui/material/Typography';
import Button from '@mui/material/Button';
import Box from '@mui/material/Box';
import Container from '@mui/material/Container';
import InputLabel from '@mui/material/InputLabel';
import TextField from '@mui/material/TextField';
import {parseJwt} from './utils'; // Import the JWT parsing function

/**
 *
 * @param {any} root0 root
 * @param {any} root0.toggleSelected root
 * @returns {any} root
 */
function Login({toggleSelected}) {
  const [credentials, setCredentials] = React.useState(
      {email: '', password: ''},
  );

  const handleInputChange = (event) => {
    const {value, name} = event.target;
    setCredentials((prevCredentials) => ({
      ...prevCredentials,
      [name]: value,
    }));
  };

  const login = (event) => {
    event.preventDefault();

    fetch('http://localhost:3010/api/v0/login', {
      method: 'POST',
      body: JSON.stringify(credentials),
      headers: {
        'Content-Type': 'application/json',
      },
    })
        .then((response) => {
          if (!response.ok) {
            throw new Error('Unauthorized');
          }
          return response.json();
        })
        .then((data) => {
          // Store the token in localStorage for future authenticated requests
          localStorage.setItem('token', data.token);

          // Parse the JWT to get user information
          const userData = parseJwt(data.token);
          toggleSelected(userData.name);
        })
        .catch((error) => {
          alert('Bad credentials try again');
        });
  };

  return (
    <Box
      sx={{
        display: 'flex',
        flexDirection: 'column',
        justifyContent: 'center',
        alignItems: 'center',
        minHeight: '100vh',
        width: '100%',
        bgcolor: '#f5f5f5',
      }}
    >
      <Container maxWidth="xs" sx={{p: 2}}>
        <Paper
          elevation={2}
          sx={{
            width: '100%',
            padding: 3,
            borderRadius: 2,
          }}
        >
          <Typography variant="h5" component="h1"
            gutterBottom align="center" sx={{mb: 3}}>
            Login
          </Typography>
          <Box
            component="form"
            onSubmit={login}
            sx={{display: 'flex', flexDirection: 'column', gap: 2}}
          >
            <Box sx={{my: 1}}>
              <InputLabel
                htmlFor="email-input"
                sx={{mb: 1, fontWeight: 'medium'}}
              >
                Email
              </InputLabel>
              <TextField
                fullWidth
                name="email"
                type="email"
                inputProps={{'aria-label': 'email'}}
                onChange={handleInputChange}
                value={credentials.email}
                required
                variant="outlined"
                size="small"
              />
            </Box>

            <Box sx={{my: 1}}>
              <InputLabel
                htmlFor="password-input"
                sx={{mb: 1, fontWeight: 'medium'}}
              >
                Password
              </InputLabel>
              <TextField
                fullWidth
                name="password"
                type="password"
                inputProps={{'aria-label': 'password'}}
                onChange={handleInputChange}
                value={credentials.password}
                required
                variant="outlined"
                size="small"
              />
            </Box>

            <Button
              type="submit"
              variant="contained"
              color="primary"
              fullWidth
              aria-label="login"
              sx={{mt: 3, py: 1.5}}
            >
              Login
            </Button>
          </Box>
        </Paper>
      </Container>
    </Box>
  );
}

Login.propTypes = {
  toggleSelected: PropTypes.func.isRequired,
};

export default Login;
