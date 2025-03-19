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
  // Set up backend server
  backend = http.createServer(app);
  backend.listen(3010, () => {
    console.log('Backend Running at http://localhost:3010');
  });

  // Set up frontend server
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
    // Use these settings for debugging:
    // headless: false,
    // slowMo: 100,
  });
  page = await browser.newPage();
  await page.goto('http://localhost:3000');

  // Login as a default user for workspace tests
  await login('molly@books.com', 'mollymember');
});

afterEach(async () => {
  const childProcess = browser.process();
  if (childProcess) {
    await childProcess.kill(9);
  }
});

// Helper functions
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

const getText = async (selector) => {
  const element = await page.waitForSelector(selector);
  const text = await element.evaluate((el) => el.textContent);
  element.dispose();
  return text;
};

// Login helper function
const login = async (email, password) => {
  await type('input[name="email"]', email);
  await type('input[name="password"]', password);
  await click('button[type="submit"]');
};

test('Molly Member can select workspaces they are in', async () => {
  await click('button[aria-label="workspaces"]');

  await page.waitForSelector('ul[role="menu"]');
  await page.waitForSelector('li[role="menuitem"]');
  const menuItems = await page.$$('li[role="menuitem"]');

  const workspaceNames = [];
  for (const item of menuItems) {
    // Extract only the span text which contains just the workspace name
    const name = await item.evaluate((el) => {
      const span = el.querySelector('span');
      return span ? span.textContent.trim() : '';
    });
    workspaceNames.push(name);
  }

  // Check for Molly's workspaces
  expect(workspaceNames.includes('CSE186')).toBe(true);
  expect(workspaceNames.includes('CSE130')).toBe(true);
  expect(workspaceNames.includes('CSE187')).toBe(true);
});
test('Anna Admin can select workspaces they are in', async () => {
  // Logout current user and login as Anna
  await click('button[aria-label="logout"]');
  await login('anna@books.com', 'annaadmin');

  await click('button[aria-label="workspaces"]');

  await page.waitForSelector('ul[role="menu"]');
  await page.waitForSelector('li[role="menuitem"]');
  const menuItems = await page.$$('li[role="menuitem"]');

  // Extract text from menu items
  const workspaceNames = [];
  for (const item of menuItems) {
    // Extract only the span text which contains just the workspace name
    const name = await item.evaluate((el) => {
      const span = el.querySelector('span');
      return span ? span.textContent.trim() : '';
    });
    workspaceNames.push(name);
  }

  // Check for Anna's workspaces
  expect(workspaceNames.includes('Staff')).toBe(true);
  expect(workspaceNames.includes('CSE186')).toBe(true);
  expect(workspaceNames.includes('CSE187')).toBe(true);
  expect(workspaceNames.includes('CSE130')).toBe(true);
});

test('Shows currently selected workspace in top left', async () => {
  await click('button[aria-label="workspaces"]');

  await page.waitForSelector('ul[role="menu"]');
  await page.waitForSelector('li[role="menuitem"]');
  const menuItems = await page.$$('li[role="menuitem"]');
  for (const item of menuItems) {
    const text = await item.evaluate((el) => el.textContent);
    if (text === 'CSE186') {
      await item.click();
      break;
    }
  }
  await page.waitForFunction(
      () => document
          .querySelector('.MuiTypography-root')?.textContent === 'CSE186',
  );

  const appBarText = await getText('.MuiTypography-root');
  expect(appBarText).toBe('CSE186');
});
