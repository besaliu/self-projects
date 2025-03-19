
/**
 *
 * @param {any} token jwt
 * @returns {any} any
 * function taken from chatgpt
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

// function taken from chatgpt
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
