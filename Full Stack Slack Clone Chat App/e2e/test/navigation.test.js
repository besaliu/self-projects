// navigation.test.js
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

const elementExists = async (selector, timeout = 5000) => {
  try {
    const element = await page.waitForSelector(selector, {timeout});
    element.dispose();
    return true;
  } catch (error) {
    void(error);
    return false;
  }
};

const login = async (email, password) => {
  await type('input[name="email"]', email);
  await type('input[name="password"]', password);
  await click('button[type="submit"]');
};

test('Back button to return to home page from channel view', async () => {
  login('molly@books.com', 'mollymember');
  await click('::-p-text(asgn1)');
  await page.waitForNetworkIdle({idleTime: 500});
  const msg1 = await page.waitForSelector(
      '::-p-text(@everyone this is where to ask questions for asgn1 of cse186)',
  );

  expect(msg1).not.toBeNull();
  await click('button[aria-label="back"]');
  await page.waitForNetworkIdle({idleTime: 500});
  const channelButton = await page.waitForSelector(
      '::-p-text(asgn2)');
  expect(channelButton).not.toBeNull();
}, 20000);

test('Menu closes when clicking button and shows channels', async () => {
  login('anna@books.com', 'annaadmin');
  await click('button[aria-label="workspaces"]');
  await page.waitForSelector('ul[role="menu"]');
  const menuOpen = await elementExists('ul[role="menu"]');
  expect(menuOpen).toBe(true);


  await click('::-p-text(CSE186)');
  await page.waitForNetworkIdle({idleTime: 500});
  const expandedAttrClosed = await page.$eval(
      'button[aria-label="workspaces"]',
      (el) => el.getAttribute('aria-expanded'));
  expect(expandedAttrClosed).toBe(null);

  const asgn1 = await page.waitForSelector('::-p-text(asgn1)');
  expect(asgn1).not.toBeNull;
  const asgn2 = await page.waitForSelector('::-p-text(asgn2)');
  expect(asgn2).not.toBeNull;
  const asgn3 = await page.waitForSelector('::-p-text(asgn3)');
  expect(asgn3).not.toBeNull;
  const gen = await page.waitForSelector('::-p-text(general)');
  expect(gen).not.toBeNull;
}, 6000);


test('Home button disabled if currently on home page', async () => {
  login('anna@books.com', 'annaadmin');
  const homeButton = await page
      .waitForSelector('button[aria-label="home"]');
  const isDisabled = await homeButton
      .evaluate((node) => node.hasAttribute('disabled'));
  expect(isDisabled).to.be.true;
}, 5000);

test('Home button brings user back to home screen', async () => {
  login('molly@books.com', 'mollymember');
  await click('::-p-text(asgn1)');
  await page.waitForNetworkIdle({idleTime: 500});
  const msg1 = await page.waitForSelector(
      '::-p-text(@everyone this is where to ask questions for asgn1 of cse186)',
  );

  expect(msg1).not.toBeNull();
  await click('button[aria-label="home"]');
  await page.waitForNetworkIdle({idleTime: 500});
  const channelButton = await page.waitForSelector(
      '::-p-text(asgn2)');
  expect(channelButton).not.toBeNull();
}, 15000);

test('Owner has O for owner', async () => {
  // Login as anna who is an admin/owner
  await login('molly@books.com', 'mollymember');

  // Open the workspace menu
  await click('button[aria-label="workspaces"]');
  await page.waitForSelector('ul[role="menu"]');

  // Check if the CSE130 workspace has an ownership indicator
  const cse130Item = await page.waitForSelector('::-p-text(CSE130)');
  const parentMenuItem = await cse130Item.evaluateHandle(
      (node) => node.closest('li[role="menuitem"]'));

  // Check for the ownership indicator (the 'O' in a circle)
  const ownershipIndicator = await parentMenuItem.evaluateHandle(
      (node) => node.querySelector('div[class*="MuiBox-root"]'),
  );

  // Verify the ownership indicator exists and contains 'O'
  const indicatorExists = await ownershipIndicator.evaluate((node) => !!node);
  expect(indicatorExists).toBe(true);

  const indicatorText = await ownershipIndicator.evaluate(
      (node) => node.textContent.trim());
  expect(indicatorText).toBe('O');
}, 6000);

test('Non Owner dislpays no O', async () => {
  // Login as molly who is a regular member
  await login('molly@books.com', 'mollymember');

  // Open the workspace menu
  await click('button[aria-label="workspaces"]');
  await page.waitForSelector('ul[role="menu"]');

  // Wait for the menu items to load
  await page.waitForSelector('li[role="menuitem"]');

  // Find menu item containing CSE186
  const menuItems = await page.$$('li[role="menuitem"]');
  let cse186MenuItem = null;

  for (const item of menuItems) {
    const text = await item.evaluate((node) => node.textContent);
    if (text.includes('CSE186')) {
      cse186MenuItem = item;
      break;
    }
  }

  expect(cse186MenuItem).not.toBeNull();

  // Check that there is no ownership indicator
  const hasOwnerIndicator = await cse186MenuItem.evaluate((node) => {
    const boxes = Array.from(node.querySelectorAll('div'));
    return boxes.some((box) => box.textContent.trim() === 'O');
  });

  expect(hasOwnerIndicator).toBe(false);
}, 10000);
