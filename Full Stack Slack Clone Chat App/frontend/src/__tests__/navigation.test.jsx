import {it, beforeAll, afterAll, afterEach, expect, vi} from 'vitest';
import {render, screen, fireEvent, waitFor}
  from '@testing-library/react';
import {setupServer} from 'msw/node';
import {http, HttpResponse} from 'msw';
import ButtonAppBar from '../AppBar';
const URL = 'http://localhost:3010/api/v0/';
const server = setupServer();
import App, {userContext, pageContext} from '../App';
beforeAll(() => server.listen());
afterEach(() => server.resetHandlers());
// Close the server after all tests are done
afterAll(() => server.close());

it('should call selectedPage when workspace is selected', async () => {
  // Mock the API response
  server.use(
      http.get(URL + 'workspace', () => {
        return HttpResponse.json([
          {workspace_id: 1, name: 'CSE186'},
          {workspace_id: 2, name: 'CSE187'},
        ]);
      }),
      http.get(URL + 'workspace/owner', (info) => {
        return HttpResponse.json({
          name: 'Molly Member',
        });
      }),

  );

  const mockSelectedPage = vi.fn();

  render(
      <userContext.Provider value="Molly Member">
        <pageContext.Provider value="none">
          <ButtonAppBar selectedPage={mockSelectedPage} />
        </pageContext.Provider>
      </userContext.Provider>,
  );

  const workspacesButton = screen.getByLabelText('workspaces');
  fireEvent.click(workspacesButton);

  await waitFor(() => screen.getByText('CSE186'));

  // Click on a workspace
  fireEvent.click(screen.getByText('CSE186'));

  expect(mockSelectedPage).toHaveBeenCalledWith('CSE186');
});

it('should close menu when an item is clicked', async () => {
  // Mock the API response
  server.use(
      http.get(URL + 'workspace', () => {
        return HttpResponse.json([
          {workspace_id: 1, name: 'CSE186'},
        ]);
      }),
      http.get(URL + 'workspace/owner', (info) => {
        return HttpResponse.json({
          name: 'Molly Member',
        });
      }),

  );

  render(
      <userContext.Provider value="Molly Member">
        <pageContext.Provider value="none">
          <ButtonAppBar selectedPage={() => {}} />
        </pageContext.Provider>
      </userContext.Provider>,
  );
  const workspacesButton = screen.getByLabelText('workspaces');
  fireEvent.click(workspacesButton);

  // Wait for the menu to open
  await waitFor(() => screen.getByText('CSE186'));

  // Verify the menu is open
  expect(screen.getByText('CSE186')).toBeInTheDocument();

  fireEvent.click(screen.getByText('CSE186'));
  await waitFor(() => {
    expect(screen.queryByText('CSE186')).not.toBeInTheDocument();
  });
});


it('should have disabled home button when on home page', async () => {
  // Mock API responses
  server.use(
      http.post(URL + 'login', () => {
        return HttpResponse.json({
          token: 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.' +
            'eyJuYW1lIjoiTW9sbHkgTWVtYmVyIiwiZW1haWwiOiJt' +
            'b2xseUBib29rcy5jb20iLCJyb2xlIjoibWVtYmVyIn0.' +
            'fake_signature',
        });
      }),

      http.get(URL + 'workspace', () => {
        return HttpResponse.json([
          {workspace_id: 1, name: 'CSE186'},
        ]);
      }),

      http.get(URL + 'channel', () => {
        return HttpResponse.json([]);
      }),
  );

  // Mock localStorage
  const localStorageMock = {
    getItem: vi.fn(),
    setItem: vi.fn(),
  };
  Object.defineProperty(window, 'localStorage', {
    value: localStorageMock,
  });

  // Render the App component
  render(<App />);

  // Login to the application
  const emailInput = screen.getByLabelText('email');
  const passwordInput = screen.getByLabelText('password');
  const loginButton = screen.getByLabelText('login');

  fireEvent.change(emailInput, {target: {value: 'test@example.com'}});
  fireEvent.change(passwordInput, {target: {value: 'password'}});
  fireEvent.click(loginButton);

  // Wait for login to complete
  await waitFor(() => screen.getByLabelText('home'));

  await waitFor(() => {
    const homeButtonAfterNav = screen.getByLabelText('home');
    expect(homeButtonAfterNav).toBeDisabled();
  });
});


it('should have enabled home button when on channel page', async () => {
  // Mock API responses
  server.use(
      http.post(URL + 'login', () => {
        return HttpResponse.json({
          token: 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.' +
            'eyJuYW1lIjoiTW9sbHkgTWVtYmVyIiwiZW1haWwiOiJt' +
            'b2xseUBib29rcy5jb20iLCJyb2xlIjoibWVtYmVyIn0.' +
            'fake_signature',
        });
      }),

      http.get(URL + 'workspace', () => {
        return HttpResponse.json([
          {workspace_id: 1, name: 'CSE186'},
        ]);
      }),

      // Add mock for channel list endpoint
      http.get(URL + 'channel', () => {
        return HttpResponse.json([
          {id: 1, name: 'asgn1'},
        ]);
      }),

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

  // Mock localStorage
  const localStorageMock = {
    getItem: vi.fn(),
    setItem: vi.fn(),
  };
  Object.defineProperty(window, 'localStorage', {
    value: localStorageMock,
  });

  // Render the App component
  render(<App />);

  // Login to the application
  const emailInput = screen.getByLabelText('email');
  const passwordInput = screen.getByLabelText('password');
  const loginButton = screen.getByLabelText('login');

  fireEvent.change(emailInput, {target: {value: 'test@example.com'}});
  fireEvent.change(passwordInput, {target: {value: 'password'}});
  fireEvent.click(loginButton);

  // Wait for channels to load
  await waitFor(() => screen.getByText('asgn1'));

  // Verify home button is initially disabled
  const homeButton = screen.getByLabelText('home');
  expect(homeButton).toBeDisabled();

  // Click on channel
  fireEvent.click(screen.getByText('asgn1'));

  // Wait for and verify home button is enabled after channel selection
  await waitFor(() => {
    const homeButtonAfterNav = screen.getByLabelText('home');
    expect(homeButtonAfterNav).not.toBeDisabled();
  });
});

it('should navigate from messages view to channels view', async () => {
  // Mock API responses
  server.use(
      http.post(URL + 'login', () => {
        return HttpResponse.json({
          token: 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.' +
            'eyJuYW1lIjoiTW9sbHkgTWVtYmVyIiwiZW1haWwiOiJt' +
            'b2xseUBib29rcy5jb20iLCJyb2xlIjoibWVtYmVyIn0.' +
            'fake_signature',
        });
      }),

      http.get(URL + 'workspace', () => {
        return HttpResponse.json([
          {workspace_id: 1, name: 'CSE186'},
        ]);
      }),

      http.get(URL + 'channel', () => {
        return HttpResponse.json([
          {id: 1, name: 'asgn1'},
        ]);
      }),

      http.get(URL + 'channel/1', () => {
        return HttpResponse.json({
          messages: [
            {id: 1, username: 'User1', content:
               'Test message', timestamp: '2024-01-01T12:00:00Z'},
          ],
        });
      }),
  );

  // Mock localStorage
  const localStorageMock = {
    getItem: vi.fn(),
    setItem: vi.fn(),
  };
  Object.defineProperty(window, 'localStorage', {
    value: localStorageMock,
  });

  // Spy on dispatchEvent to verify custom events
  const dispatchEventSpy = vi.spyOn(window, 'dispatchEvent');

  // Render the App component
  render(<App />);

  // Login to the application
  const emailInput = screen.getByLabelText('email');
  const passwordInput = screen.getByLabelText('password');
  const loginButton = screen.getByLabelText('login');

  fireEvent.change(emailInput, {target: {value: 'test@example.com'}});
  fireEvent.change(passwordInput, {target: {value: 'password'}});
  fireEvent.click(loginButton);

  // Wait for login to complete and channels to load
  await waitFor(() => screen.getByText('asgn1'));

  // Click on a channel to navigate to messages view
  fireEvent.click(screen.getByText('asgn1'));

  // Wait for the message to appear
  await waitFor(() => screen.getByText('Test message'));

  // Verify that the viewChanged event was dispatched with isChannelView=true
  expect(dispatchEventSpy).toHaveBeenCalledWith(
      expect.objectContaining({
        type: 'viewChanged',
        detail: expect.objectContaining({
          isChannelView: true,
        }),
      }),
  );

  // Verify home button is enabled
  const homeButton = screen.getByLabelText('home');
  expect(homeButton).not.toBeDisabled();

  // Click the home button to return to channels view
  fireEvent.click(homeButton);

  // Verify that the returnToChannelsList event was dispatched
  expect(dispatchEventSpy).toHaveBeenCalledWith(
      expect.objectContaining({
        type: 'returnToChannelsList',
      }),
  );

  // Check that the UI returned to channels view by looking for channels heading
  await waitFor(() => screen.getByText('Channels'));

  // Verify that updateAppBarTitle event was dispatched with the workspace name
  expect(dispatchEventSpy).toHaveBeenCalledWith(
      expect.objectContaining({
        type: 'updateAppBarTitle',
        detail: expect.objectContaining({
          title: 'CSE186',
          type: 'workspace',
        }),
      }),
  );

  await waitFor(() => {
    const homeButtonAfterNav = screen.getByLabelText('home');
    expect(homeButtonAfterNav).toBeDisabled();
  });
});

it('should update workspace and reset channel view ', async () => {
  server.use(
      http.post(URL + 'login', () => {
        return HttpResponse.json({
          token: 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.' +
            'eyJuYW1lIjoiTW9sbHkgTWVtYmVyIiwiZW1haWwiOiJt' +
            'b2xseUBib29rcy5jb20iLCJyb2xlIjoibWVtYmVyIn0.' +
            'fake_signature',
        });
      }),

      http.get(URL + 'workspace', () => {
        return HttpResponse.json([
          {workspace_id: 1, name: 'CSE186'},
          {workspace_id: 2, name: 'CSE187'},
        ]);
      }),
      http.get(URL + 'workspace/owner', (info) => {
        return HttpResponse.json({
          name: 'Molly Member',
        });
      }),

      http.get(URL + 'channel', () => {
        return HttpResponse.json([
          {id: 1, name: 'general'},
        ]);
      }),
  );

  render(<App />);

  // Login to the application
  const emailInput = screen.getByLabelText('email');
  const passwordInput = screen.getByLabelText('password');
  const loginButton = screen.getByLabelText('login');

  fireEvent.change(emailInput, {target: {value: 'test@example.com'}});
  fireEvent.change(passwordInput, {target: {value: 'password'}});
  fireEvent.click(loginButton);
  await waitFor(() => screen.getByLabelText('workspaces'));
  fireEvent.click(screen.getByLabelText('workspaces'));

  await waitFor(() => screen.getByText('CSE187'));
  fireEvent.click(screen.getByText('CSE187'));
  await waitFor(() => {
    expect(screen.getByText('CSE187')).toBeInTheDocument();
  });
  const homeButton = screen.getByLabelText('home');
  expect(homeButton).toBeDisabled();
});
