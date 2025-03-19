import jwt from 'jsonwebtoken';
import dotenv from 'dotenv';

dotenv.config();

/**
 *
 * @param {any} req req
 * @param {any} res res
 * @param {any} next next
 * @returns {any} jwt
 */
export function authenticateJWT(req, res, next) {
  // Get the authorization header
  const authHeader = req.headers.authorization;

  // Check if it follows the Bearer token format
  const parts = authHeader.split(' ');
  const token = parts[1];

  try {
    // Verify the token
    const decoded = jwt.verify(token, process.env.JWT_SECRET ||
        'your-secret-key');

    // Add the decoded payload to the request object
    req.user = decoded;

    // Continue to the next middleware or route handler
    next();
  } catch (error) {
    if (error.name === 'TokenExpiredError') {
      return res.status(401).send();
    }
  }
}
