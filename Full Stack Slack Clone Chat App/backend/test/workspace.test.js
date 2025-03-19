
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

it('get workspaces for member', async () => {
  const response = await request
      .get('/api/v0/workspace?username=Molly%20Member')
      .set('Authorization', `Bearer ${authToken}`);

  expect(response.status).toBe(200);
});

it('get workspaces for non auth member fails', async () => {
  const response = await request
      .get('/api/v0/workspace?username=Molly%20Member');

  expect(response.status).toBe(401);
});

it('fails get workspaces for non existent member', async () => {
  const response = await request
      .get('/api/v0/workspace?username=Mol%20Mem')
      .set('Authorization', `Bearer ${authToken}`);

  expect(response.status).toBe(403);
});


it('fails get workspaces for existing member w/ wrong token', async () => {
  const response = await request
      .get('/api/v0/workspace?username=Anna%20Admin')
      .set('Authorization', `Bearer ${authToken}`);

  expect(response.status).toBe(403);
});


it('get channels for workspace', async () => {
  const response = await request
      .get('/api/v0/channel?workspace=CSE186')
      .set('Authorization', `Bearer ${authToken}`);

  expect(response.status).toBe(200);
});

it('fails get channels for non authenticated workspace', async () => {
  const response = await request
      .get('/api/v0/channel?workspace=CSE186');

  expect(response.status).toBe(401);
});

it('fails get channels for nonexistent workspace', async () => {
  const response = await request
      .get('/api/v0/channel?workspace=CSE116')
      .set('Authorization', `Bearer ${authToken}`);

  expect(response.status).toBe(403);
});

it('get workspace owner - successful retrieval', async () => {
  // First ensure Molly Member has access to CSE186 workspace
  const workspacesResponse = await request
      .get('/api/v0/workspace?username=Molly%20Member')
      .set('Authorization', `Bearer ${authToken}`);

  expect(workspacesResponse.status).toBe(200);
  expect(workspacesResponse.body.some((w) => w.name === 'CSE186')).toBe(true);

  // Now test the workspace owner endpoint
  const response = await request
      .get('/api/v0/workspace/owner?workspace=CSE186')
      .set('Authorization', `Bearer ${authToken}`);

  expect(response.status).toBe(200);
  expect(response.body).toHaveProperty('name');
  expect(response.body.name).toBe('Anna Admin');
});

it('fails to get workspace owner - unauthenticated request', async () => {
  const response = await request
      .get('/api/v0/workspace/owner?workspace=CSE186');

  expect(response.status).toBe(401);
});

it('fails to get workspace owner - unauthorized user', async () => {
  // Assuming there's a workspace that Molly doesn't have access to
  const response = await request
      .get('/api/v0/workspace/owner?workspace=PrivateWorkspace')
      .set('Authorization', `Bearer ${authToken}`);

  expect(response.status).toBe(403);
});

it('fails to get workspace owner - workspace doesn\'t exist', async () => {
  const response = await request
      .get('/api/v0/workspace/owner?workspace=NonExistentWorkspace')
      .set('Authorization', `Bearer ${authToken}`);

  expect(response.status).toBe(403);
});
