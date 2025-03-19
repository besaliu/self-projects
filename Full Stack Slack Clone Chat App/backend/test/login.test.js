
import {it, beforeAll, afterAll, expect} from 'vitest';
import supertest from 'supertest';
import http from 'http';

import * as db from './db.js';
import app from '../src/app.js';

let server;
let request;
let authToken;

// Helper function to get authentication token
/**
 * @returns {any} authtoken
 */
async function getAuthToken() {
  if (authToken) return authToken;

  const loginResponse = await request
      .post('/api/v0/login')
      .send({
        email: 'molly@books.com',
        password: 'mollymember',
      });

  expect(loginResponse.status).toBe(200);
  expect(loginResponse.body).toHaveProperty('token');

  authToken = loginResponse.body.token;
  return authToken;
}

beforeAll(async () => {
  server = http.createServer(app);
  server.listen();
  request = supertest(server);
  await db.reset();
  return getAuthToken();
});

afterAll(async () => {
  db.close();
  await server.close();
});

it('Errors on Invalid URL', async () => {
  await request.get('/api/v0/so-not-a-real-end-point')
      .expect(404);
});

it('login with good credentials', async () => {
  const response = await request
      .post('/api/v0/login')
      .send({
        'email': 'molly@books.com',
        'password': 'mollymember',
      });

  expect(response.body.token.length).toBeGreaterThan(0);
});


it('login with bad credentials', async () => {
  const response = await request
      .post('/api/v0/login')
      .send({
        'email': 'molly@bookss.com',
        'password': 'mollymembers',
      });

  expect(response.status).toBe(401);
});
