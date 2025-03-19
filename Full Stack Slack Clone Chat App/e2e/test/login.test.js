import {test, beforeAll, afterAll, beforeEach, afterEach, expect} from 'vitest';
import puppeteer from 'puppeteer';
import path from 'path';
import express from 'express';
import http from 'http';

import 'dotenv/config';
import app from '../../backend/src/app.js';

let backend;
let frontend;
let browser;
let page;

beforeAll(() => {
  backend = http.createServer(app);
  backend.listen(3010, () => {
    console.log('Backend Running at http://localhost:3010');
  });

  frontend = http.createServer(
      express()
          .use('/assets', express.static(
              path.join(__dirname, '..', '..', 'frontend', 'dist', 'assets')))
          .get('*', function(req, res) {
            res.sendFile('index.html',
                {root: path.join(__dirname, '..', '..', 'frontend', 'dist')});
          }),
  );
  frontend.listen(3000, () => {
    console.log('Frontend Running at http://localhost:3000');
  });
});

afterAll(async () => {
  await backend.close();
  await frontend.close();
  setImmediate(function() {
    frontend.emit('close');
  });
});

beforeEach(async () => {
  browser = await puppeteer.launch({
    headless: true,
  });
  page = await browser.newPage();
  await page.goto('http://localhost:3000');
});

afterEach(async () => {
  const childProcess = browser.process();
  if (childProcess) {
    await childProcess.kill(9);
  }
});

const click = async (selector) => {
  const clickable = await page.waitForSelector(selector);
  await clickable.click();
  clickable.dispose();
};

const type = async (selector, text) => {
  const input = await page.waitForSelector(selector);
  await input.type(text);
  input.dispose();
};

const elementExists = async (selector, timeout = 5000) => {
  const element = await page.waitForSelector(selector, {timeout});
  element.dispose();
  return true;
};

const login = async (email, password) => {
  await type('input[name="email"]', email);
  await type('input[name="password"]', password);
  await click('button[type="submit"]');
};

test('Initial Login View', async () => {
  // Check for login form elements
  const emailInput = await elementExists('input[name="email"]');
  const passwordInput = await elementExists('input[name="password"]');
  const loginButton = await elementExists('button[type="submit"]');

  expect(emailInput).toBe(true);
  expect(passwordInput).toBe(true);
  expect(loginButton).toBe(true);
});

test('User can login with Molly Member', async () => {
  login('molly@books.com', 'mollymember');
  const logout = await page.waitForSelector('button[aria-label="logout"]');
  expect(logout).not.toBeNull;
});

test('User can login with Anna Admin', async () => {
  login('anna@books.com', 'annaadmin');

  const logout = await page.waitForSelector('button[aria-label="logout"]');
  expect(logout).not.toBeNull;
});

test('Login with invalid credentials fails', async () => {
  await type('input[name="email"]', 'wrong@example.com');
  await type('input[name="password"]', 'wrongpassword');

  await click('button[type="submit"]');
  const emailInput = await elementExists('input[name="email"]');
  expect(emailInput).toBe(true);
});

test('Logout button functionality', async () => {
  login('molly@books.com', 'mollymember');
  // Click logout button
  await click('button[aria-label="logout"]');

  // Verify we are back at login page
  const emailInput = await elementExists('input[name="email"]');
  expect(emailInput).toBe(true);
});
