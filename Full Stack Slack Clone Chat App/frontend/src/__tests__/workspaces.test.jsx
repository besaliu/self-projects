import {it, beforeAll, afterAll, afterEach, expect, vi} from 'vitest';
import {render, screen, fireEvent, waitFor}
  from '@testing-library/react';
import {setupServer} from 'msw/node';
import {http, HttpResponse} from 'msw';
import ButtonAppBar from '../AppBar';
const URL = 'http://localhost:3010/api/v0/';
const server = setupServer();
import {userContext, pageContext} from '../App';
beforeAll(() => server.listen());
afterEach(() => server.resetHandlers());
// Close the server after all tests are done
afterAll(() => server.close());

it('fetching workspaces for user', async () => {
  // Mock the GET request for the workspace endpoint
  server.use(
      http.get(URL + 'workspace', () => {
        // Return an array of workspaces
        return HttpResponse.json([
          {
            workspace_id: 1,
            name: 'CSE186',
          },
          {
            workspace_id: 2,
            name: 'CSE187',
          },
        ]);
      }),

      // Also mock the owner endpoint that's called for each workspace
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

  const select = screen.getByLabelText('workspaces');
  fireEvent.click(select);

  await waitFor(() => screen.getByText('CSE186'));
  expect(screen.getByText('CSE187')).toBeInTheDocument();
});

it('sets isOwner to false for not owner response', async () => {
  // Mock the GET request for the workspace endpoint
  server.use(
      http.get(URL + 'workspace', () => {
      // Return an array of workspaces
        return HttpResponse.json([
          {
            workspace_id: 1,
            name: 'CSE186',
          },
        ]);
      }),

      // Mock the owner endpoint to return a non-ok response
      http.get(URL + 'workspace/owner', () => {
        return new HttpResponse(null, {
          status: 404,
          statusText: 'Not Found',
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

  const select = screen.getByLabelText('workspaces');
  fireEvent.click(select);

  // Wait for the workspace to appear in the menu
  await waitFor(() => screen.getByText('CSE186'));

  // Verify the 'O' ownership indicator is not present
  expect(screen.queryByText('O')).not.toBeInTheDocument();
});
