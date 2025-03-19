// utils.test.js, chatgpt helped generate to test utilities
import {expect, describe, it, vi, beforeEach, afterEach} from 'vitest';

/**
 *
 * @param {any} token jwt
 * @returns {any} any
 */
export function parseJwt(token) {
  const base64Url = token.split('.')[1];
  const base64 = base64Url.replace(/-/g, '+').replace(/_/g, '/');
  const jsonPayload = decodeURIComponent(
      atob(base64)
          .split('')
          .map((c) => '%' + ('00' + c.charCodeAt(0).toString(16)).slice(-2))
          .join(''),
  );
  return JSON.parse(jsonPayload);
}

/**
 *
 * @param {any} url url
 * @param {any} options options
 * @returns {any} any
 */
export function authenticatedFetch(url, options = {}) {
  const token = localStorage.getItem('token');

  const authOptions = {
    ...options,
    headers: {
      ...options.headers,
      'Authorization': `Bearer ${token}`,
    },
  };

  return fetch(url, authOptions)
      .then((response) => {
        return response;
      });
}


describe('parseJwt', () => {
  it('should correctly parse a valid JWT token', () => {
    // Format: header.payload.signature
    const mockPayload = {
      id: '12345',
      name: 'Test User',
      email: 'test@example.com',
      role: 'user',
    };

    const encodedPayload = btoa(JSON.stringify(mockPayload));
    const mockToken = `header.${encodedPayload}.signature`;

    const result = parseJwt(mockToken);

    expect(result).toEqual(mockPayload);
  });

  it('should handle invalid token format', () => {
    // Test with malformed tokens
    expect(() => parseJwt('invalid-token')).toThrow();
    expect(() => parseJwt('')).toThrow();
    expect(() => parseJwt(null)).toThrow();
    expect(() => parseJwt(undefined)).toThrow();
  });
});

describe('authenticatedFetch', () => {
  let originalFetch;
  let originalLocalStorage;

  beforeEach(() => {
    // Mock the global fetch
    originalFetch = global.fetch;
    global.fetch = vi.fn();

    // Mock localStorage
    originalLocalStorage = global.localStorage;
    global.localStorage = {
      getItem: vi.fn(),
      setItem: vi.fn(),
      removeItem: vi.fn(),
    };
  });

  afterEach(() => {
    // Restore original implementations
    global.fetch = originalFetch;
    global.localStorage = originalLocalStorage;
  });

  it('should add authorization header when token exists', async () => {
    const mockToken = 'mock-jwt-token';
    const mockUrl = 'http://localhost:3010/api/v0/test';
    const mockResponse = {json: () => Promise.resolve({data: 'success'})};

    // Setup mocks
    localStorage.getItem.mockReturnValue(mockToken);
    fetch.mockResolvedValue(mockResponse);

    // Call the function
    await authenticatedFetch(mockUrl);

    // Verify fetch was called with the correct headers
    expect(fetch).toHaveBeenCalledWith(mockUrl, {
      headers: {
        'Authorization': `Bearer ${mockToken}`,
      },
    });
  });
  it('should handle fetch options correctly', async () => {
    const mockToken = 'mock-jwt-token';
    const mockUrl = 'http://localhost:3010/api/v0/test';
    const mockOptions = {
      method: 'POST',
      body: JSON.stringify({test: true}),
      headers: {
        'Content-Type': 'application/json',
      },
    };
    const mockResponse = {json: () => Promise.resolve({data: 'success'})};
    localStorage.getItem.mockReturnValue(mockToken);
    fetch.mockResolvedValue(mockResponse);

    await authenticatedFetch(mockUrl, mockOptions);

    // Verify fetch was called with merged options and headers
    expect(fetch).toHaveBeenCalledWith(mockUrl, {
      method: 'POST',
      body: JSON.stringify({test: true}),
      headers: {
        'Content-Type': 'application/json',
        'Authorization': `Bearer ${mockToken}`,
      },
    });
  });

  it('should handle fetch errors', async () => {
    const mockUrl = 'http://localhost:3010/api/v0/test';

    // Setup fetch to reject with an error
    fetch.mockRejectedValue(new Error('Network error'));

    // Call the function and expect it to throw
    await expect(authenticatedFetch(mockUrl)).rejects.toThrow('Network error');
  });
});
