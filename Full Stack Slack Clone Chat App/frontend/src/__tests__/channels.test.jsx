import {it, beforeAll, afterAll, afterEach, expect} from 'vitest';
import {render, screen, fireEvent, waitFor}
  from '@testing-library/react';
import {setupServer} from 'msw/node';
import {http, HttpResponse} from 'msw';
import App from '../App';
import Channels from '../Channel';
const URL = 'http://localhost:3010/api/v0/';
const server = setupServer();
import {pageContext} from '../App';
beforeAll(() => server.listen());
afterEach(() => server.resetHandlers());
// Close the server after all tests are done
afterAll(() => server.close());
const mockJwt = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.' +
                'eyJuYW1lIjoiTW9sbHkgTWVtYmVyIiwiZW1haWwiOiJt' +
                'b2xseUBib29rcy5jb20iLCJyb2xlIjoibWVtYmVyIn0.' +
                'fake_signature';

it('fetches and displays channels for the selected workspace', async () => {
  // Mock the GET request for channels
  server.use(
      http.get(URL + 'channel', (req) => {
        return HttpResponse.json([
          {id: 1, name: 'general'},
          {id: 2, name: 'random'},
        ]);
      }),
  );

  render(
      <pageContext.Provider value="CSE186">
        <Channels />
      </pageContext.Provider>,
  );

  await waitFor(() => {
    expect(screen.getByText('general')).toBeInTheDocument();
    expect(screen.getByText('random')).toBeInTheDocument();
  });
});


it('fetches messages when a channel is selected', async () => {
  // Mock both API endpoints needed for the test
  server.use(
      http.get(URL + 'channel', ({request}) => {
        return HttpResponse.json([
          {id: 1, name: 'general'},
          {id: 2, name: 'random'},
        ]);
      }),
  );

  server.use(
      http.get(URL + 'channel/1', () => {
        return HttpResponse.json({
          messages: [
            {id: 101, username: 'User1', content: 'Test message 1',
              timestamp: '2023-05-15T14:30:00Z'},
            {id: 102, username: 'User2', content: 'Test message 2',
              timestamp: '2023-05-15T14:35:00Z'},
          ],
        });
      }),
  );

  render(
      <pageContext.Provider value="CSE186">
        <Channels />
      </pageContext.Provider>,
  );

  await waitFor(() => {
    const channelItems = screen.getAllByText(/general|random/);
    expect(channelItems.length).toBe(2);
  });
  const generalChannel = screen.getByText('general');
  fireEvent.click(generalChannel.closest('div'));

  await waitFor(() => {
    expect(screen.getByText('Test message 1')).toBeInTheDocument();
    expect(screen.getByText('Test message 2')).toBeInTheDocument();
    expect(screen.getByText('User1')).toBeInTheDocument();
    expect(screen.getByText('User2')).toBeInTheDocument();
  });
});

it('handles no messages', async () => {
  server.use(
      http.get(URL + 'channel', ({request}) => {
        return HttpResponse.json([
          {id: 1, name: 'general'},
          {id: 2, name: 'random'},
        ]);
      }),
  );

  server.use(
      http.get(URL + 'channel/1', () => {
        return HttpResponse.json({
        });
      }),
  );

  render(
      <pageContext.Provider value="CSE186">
        <Channels />
      </pageContext.Provider>,
  );

  // Wait for channels to load
  await waitFor(() => {
    const channelItems = screen.getAllByText(/general|random/);
    expect(channelItems.length).toBe(2);
  });
  const generalChannel = screen.getByText('general');
  fireEvent.click(generalChannel.closest('div'));

  // Verify the messages were fetched and displayed
  await waitFor(() => {
    expect(screen.getByText('No messages in this channel')).toBeInTheDocument();
  });
});

it('back button works throughout the app navigation flow', async () => {
  server.use(
      http.post(URL + 'login', () => {
        return HttpResponse.json({
          token: mockJwt,
        });
      }),
  );

  // Mock workspace endpoint
  server.use(
      http.get(URL + 'workspace', () => {
        return HttpResponse.json([
          {workspace_id: 1, name: 'CSE186'},
          {workspace_id: 2, name: 'Work'},
        ]);
      }),
  );

  // Mock channels endpoint
  server.use(
      http.get(URL + 'channel', () => {
        return HttpResponse.json([
          {id: 1, name: 'general'},
          {id: 2, name: 'random'},
        ]);
      }),
  );

  // Mock channel messages endpoint
  server.use(
      http.get(URL + 'channel/1', () => {
        return HttpResponse.json({
          messages: [
            {id: 101, username: 'User1', content: 'Test message 1',
              timestamp: '2023-05-15T14:30:00Z'},
            {id: 102, username: 'User2', content: 'Test message 2',
              timestamp: '2023-05-15T14:35:00Z'},
          ],
        });
      }),
  );

  render(<App />);

  // Login
  const emailInput = screen.getByLabelText('email');
  const passwordInput = screen.getByLabelText('password');
  const loginButton = screen.getByLabelText('login');

  fireEvent.change(emailInput, {target: {value: 'molly@books.com'}});
  fireEvent.change(passwordInput, {target: {value: 'password123'}});
  fireEvent.click(loginButton);

  // Wait for successful login
  await screen.findByLabelText('logout');
  expect(screen.getByLabelText('home')).toBeInTheDocument();


  // Verify the workspace was selected
  await screen.findByText('Channels');

  // Find and click on the general channel
  const generalChannel = await screen.findByText('general');
  fireEvent.click(generalChannel);

  // Verify we entered the message view
  await screen.findByText('Test message 1');
  await screen.findByText('Test message 2');

  // Find and click the back button (it appears in the AppBar)
  // Using an approach based on your components
  const backButton = screen.getAllByRole('button')[0]; // First button in AppBar
  fireEvent.click(backButton);

  // Verify we're back to the channel list
  await screen.findByText('Channels');
  expect(screen.queryByText('Test message 1')).not.toBeInTheDocument();
});

it('formats message dates as Today, Yesterday, or date format', async () => {
  // Get current date for test data
  const today = new Date();
  const yesterday = new Date(today);
  yesterday.setDate(yesterday.getDate() - 1);
  const twoDaysAgo = new Date(today);
  twoDaysAgo.setDate(today.getDate() - 2);

  // Format dates for test data
  const todayISO = today.toISOString();
  const yesterdayISO = yesterday.toISOString();
  const twoDaysAgoISO = twoDaysAgo.toISOString();

  // Expected formatted date for two days ago (e.g., "Monday, March 11")
  const options = {weekday: 'long', month: 'long', day: 'numeric'};
  const expectedFormattedDate = twoDaysAgo
      .toLocaleDateString(undefined, options);

  // Mock the channel list endpoint
  server.use(
      http.get(URL + 'channel', () => {
        return HttpResponse.json([
          {id: 1, name: 'general'},
        ]);
      }),
  );

  // Mock the channel messages with different dates
  server.use(
      http.get(URL + 'channel/1', () => {
        return HttpResponse.json({
          messages: [
            {id: 101, username: 'User1', content:
              'Today message', timestamp: todayISO},
            {id: 102, username: 'User2', content:
              'Yesterday message', timestamp: yesterdayISO},
            {id: 103, username: 'User3', content:
              'Two days ago message', timestamp: twoDaysAgoISO},
          ],
        });
      }),
  );

  render(
      <pageContext.Provider value="CSE186">
        <Channels />
      </pageContext.Provider>,
  );

  // Click on general channel
  const generalChannel = await screen.findByText('general');
  fireEvent.click(generalChannel);

  // Verify messages are displayed
  await screen.findByText('Today message');
  await screen.findByText('Yesterday message');
  await screen.findByText('Two days ago message');

  // Verify date headers are correctly formatted
  await screen.findByText('Today');
  await screen.findByText('Yesterday');
  await screen.findByText(expectedFormattedDate);
});
