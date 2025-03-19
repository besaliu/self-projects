
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

it('getting channel messages works', async () => {
  const response = await request
      .get('/api/v0/channel/a14f33a6-e9e5-4b92-b4f3-9204b515343c')
      .set('Authorization', `Bearer ${authToken}`);

  expect(response.status).toBe(200);
});

it('getting non auth channel messages fails', async () => {
  const response = await request
      .get('/api/v0/channel/a14f33a6-e9e5-4b92-b4f3-9204b515343c');

  expect(response.status).toBe(401);
});

it('getting nonexistent channel messages fails', async () => {
  const response = await request
      .get('/api/v0/channel/a14f33a6-e9e5-4b92-b4f3-9204b515342c')
      .set('Authorization', `Bearer ${authToken}`);

  expect(response.status).toBe(403);
});

it('rejects expired JWT tokens', async () => {
  const jwt = require('jsonwebtoken');

  // Create a JWT that expires immediately code taken from ChatGPT
  const shortLivedToken = jwt.sign(
      {userId: 'testuser', email: 'molly@books.com'},
      process.env.JWT_SECRET || 'your-secret-key',
      {expiresIn: '1s'},
  );

  await new Promise((resolve) => setTimeout(resolve, 1500));
  const response = await request
      .get('/api/v0/workspace?username=Molly%20Member')
      .set('Authorization', `Bearer ${shortLivedToken}`);

  console.log('Status:', response.status);
  console.log('Response body:', JSON.stringify(response.body));

  expect(response.status).toBe(401);
}, 5000);

