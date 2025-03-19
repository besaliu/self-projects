import {it, beforeAll, afterAll, afterEach, expect, vi} from 'vitest';
import {render, screen, fireEvent, waitFor}
  from '@testing-library/react';
import {setupServer} from 'msw/node';
import {http, HttpResponse} from 'msw';
import App from '../App';
const URL = 'http://localhost:3010/api/v0/';
const server = setupServer();

const mockJwt = 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.' +
                'eyJuYW1lIjoiTW9sbHkgTWVtYmVyIiwiZW1haWwiOiJt' +
                'b2xseUBib29rcy5jb20iLCJyb2xlIjoibWVtYmVyIn0.' +
                'fake_signature';
beforeAll(() => server.listen());
afterEach(() => server.resetHandlers());
// Close the server after all tests are done
afterAll(() => server.close());
it('logout works', async () => {
  // Mock the POST request for the login endpoint
  server.use(
      http.post(URL + 'login', (req, res, ctx) => {
        return HttpResponse.json(
            {
              token: mockJwt,
            },
        );
      }),
      http.get(URL + 'workspace', (req, res, ctx) => {
        return HttpResponse.json(
            [],
        );
      }),
  );

  render(<App />);

  const emailInput = screen.getByLabelText('email');
  const passwordInput = screen.getByLabelText('password');
  const loginButton = screen.getByLabelText('login');

  fireEvent.change(emailInput, {target: {value: 'molly@books.com'}});
  fireEvent.change(passwordInput, {target: {value: 'password123'}});

  fireEvent.click(loginButton);

  await waitFor(() => screen.getByLabelText('logout'));
  const logout = screen.getByLabelText('logout');

  fireEvent.click(logout);
  expect(screen.getByLabelText('login')).toBeInTheDocument();
});

it('login with wrong credentials', async () => {
  // Mock the POST request to return a 401 status
  server.use(
      http.post(URL + 'login', (req, res, ctx) => {
        return res(ctx.status(401));
      }),
  );

  // Mock window.alert since it's used in the login function
  const alertMock = vi.spyOn(window, 'alert').mockImplementation(() => {});

  render(<App />);

  const emailInput = screen.getByLabelText('email');
  const passwordInput = screen.getByLabelText('password');
  const loginButton = screen.getByLabelText('login');

  fireEvent.change(emailInput, {target: {value: 'wrong@user.com'}});
  fireEvent.change(passwordInput, {target: {value: 'wrongpassword'}});

  fireEvent.click(loginButton);

  await waitFor(() => {
    expect(alertMock).toHaveBeenCalledWith('Bad credentials try again');
  });

  alertMock.mockRestore(); // Clean up mock after test
});
