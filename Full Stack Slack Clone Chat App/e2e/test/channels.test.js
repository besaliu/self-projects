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
    slowMo: 10,
  });
  page = await browser.newPage();
  await page.goto('http://localhost:3000');

  // Login and navigate to a workspace for channel tests
  await setupTestEnvironment();
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

// Helper function to login and select workspace
const setupTestEnvironment = async () => {
  // Login as Molly
  await type('input[name="email"]', 'molly@books.com');
  await type('input[name="password"]', 'mollymember');
  await click('button[type="submit"]');
  await click('button[aria-label="workspaces"]');
  await page.waitForSelector('ul[role="menu"]');
  await page.waitForSelector('li[role="menuitem"]');
  const menuItems = await page.$$('li[role="menuitem"]');
  for (const item of menuItems) {
    const text = await item.evaluate((el) => el.textContent);
    if (text === 'CSE187') {
      await item.click();
      break;
    }
  }
  await page.waitForNetworkIdle({idleTime: 500});
};

test('Selecting a workspace shows a list of the channels in it', async () => {
  const channelsHeading = await getText('h6');
  expect(channelsHeading).toBe('Channels');
  const asgn1 = await page.waitForSelector('::-p-text(asgn1)');
  expect(asgn1).not.toBeNull;
  const asgn2 = await page.waitForSelector('::-p-text(asgn2)');
  expect(asgn2).not.toBeNull;
  const asgn3 = await page.waitForSelector('::-p-text(asgn3)');
  expect(asgn3).not.toBeNull;
  const gen = await page.waitForSelector('::-p-text(general)');
  expect(gen).not.toBeNull;
}, 6000);

test('Selecting a channel shows a list of the messages in it', async () => {
  await click('::-p-text(asgn1)');
  await page.waitForNetworkIdle({idleTime: 500});

  const msg1 = await page.waitForSelector(
      '::-p-text(@everyone this is where to ask questions for asgn1 of cse187)',
  );

  expect(msg1).not.toBeNull();
}, 7000);

test('Shows which channel is being shown in top left', async () => {
  await click('::-p-text(asgn1)');
  await page.waitForNetworkIdle({idleTime: 500});

  const appBarText = await getText('.MuiTypography-root');
  expect(appBarText).toBe('asgn1');
}, 6000);
// chatgpt aided in writing this test
test('Messages are sorted by date then by time, recent at bottom', async () => {
  await click('::-p-text(asgn1)');
  await page.waitForNetworkIdle({idleTime: 500});
  await page.waitForSelector(
      'div::-p-text(@everyone this is where to ask questions for asgn1)');

  const firstMsg = await page.waitForSelector(
      '::-p-text(@everyone this is where to ask questions for asgn1 of cse187)',
  );
  const secondMsg = await page.waitForSelector(
      '::-p-text(Has anyone started the assignment yet?)');
  const thirdMsg = await page.waitForSelector(
      '::-p-text(Yes, I just finished part 1. ' +
  'Let me know if you have questions!)',
  );
  const fourthMsg = await page.waitForSelector(
      '::-p-text(Thanks! Im struggling with the database schema part.)');

  const firstPosition = await firstMsg.boundingBox();
  const secondPosition = await secondMsg.boundingBox();
  const thirdPosition = await thirdMsg.boundingBox();
  const fourthPosition = await fourthMsg.boundingBox();
  expect(firstPosition.y).toBeLessThan(secondPosition.y);
  expect(secondPosition.y).toBeLessThan(thirdPosition.y);
  expect(thirdPosition.y).toBeLessThan(fourthPosition.y);
}, 10000);
